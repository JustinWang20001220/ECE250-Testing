# from flask_restful import Api
import time
import os, subprocess, re, threading
import queue
from flask import Flask, request, render_template, jsonify, make_response
from flask_cors import CORS

from sqlalchemy.orm.session import sessionmaker

from database import Base, engine, Projects, Tests, TestFiles
from utils import publicate

app = Flask(__name__)
CORS(app)

test_queue = queue.Queue()
results = {}
lock = threading.Lock()

# Base.metadata.create_all(engine)
DB_Session = sessionmaker(bind=engine)
db_session = DB_Session()

dir_id = 0
stage_num = 0


# @app.route("/")
# def index():
#     # Maybe render the html with description of the tests.
#     # The webpage should be able to make request to the run_test api
#     # and display the results on the same page
#     return render_template("online_tester.html")

@app.route("/api/run_test", methods=["POST"])
def run_test():
    if request.method == "POST":
        if "random_id" not in request.form:
            print("id not found")
            return make_response(jsonify(test_result = "Oops No id"), 200)

        if "h_file" not in request.files:
            # print(request.files)
            return make_response(jsonify(test_result = "Oops no file received"), 200)

        h_file = request.files["h_file"]
        random_id = request.form["random_id"]
        h_file.save(f"./submissions/{random_id}.h")

        # copy uploaded file into testing environment
        subprocess.run(["cp", f"./submissions/{random_id}.h", "./test_space/Search_tree.h"])
        publicate("./test_space/Search_tree.h")
 
        # compile the test
        compile = subprocess.run(["g++", "-std=c++11", "./test_space/test_v1.cpp", "-o", f"./test_space/test{random_id}"])   
        if compile.returncode != 0:
            subprocess.run(["rm", "-f", f"./submissions/{random_id}.h", f"./test_space/Search_tree.h"])
            return jsonify({"test_result": "cannot compile\n", "segmentation_fault": False})

        # run the test and send results back to the client
        try:
            test = subprocess.run(f"./test_space/test{random_id}", stdout=subprocess.PIPE, text=True, timeout=20)
        except subprocess.TimeoutExpired:
            result = "Test ran too long. Your program may have entered an infinite loop.\n"
        is_faulty = test.returncode != 0
        result = test.stdout

        # delete file
        subprocess.run(["rm", "-f", f"./submissions/{random_id}.h", f"./test_space/test{random_id}", "./test_space/Search_tree.h"])

        return jsonify({"test_result": result, "segmentation_fault": is_faulty})


# # Api that returns the available project id's
# @app.route("api/get_projects", method=["GET"])
# def get_projects():
#     pass


# Api that takes in project_id and returns the tests (name + id)
@app.route("/api/get_all_tests", methods=["GET"])
def get_all_tests():
    tests = db_session.query(Tests).all()
    return jsonify({
        "new_tests": [ {"test_id": test.id, "test_name": test.test_name} for test in tests]
    })


# Api: live search for test names
@app.route("/api/search_test/<string>", methods=["POST", "GET"])
def search_test(string):
    print(f"Searching {string}\n")
    similar_tests = db_session.query(Tests).filter(Tests.test_name.ilike("%" + string + "%")).all()
    for test in similar_tests:
        print(test.test_name)
    # sql = f"SELECT * FROM tests WHERE test_name LIKE '%{input}%'"
    # similar_tests = db_session.execute(text(sql)).fetchall()

    return jsonify({
        "new_tests" : [ {"test_id": test.id, "test_name": test.test_name} for test in similar_tests]
    })


# Api that takes one parameters: test_id
# input form: 
@app.route("/api/run_selected", methods=["POST"])
def run_selected():
    files = request.files.getlist("file")
    for file in files:
        file.save(os.path.join(app.config["TESTING_DIR"], file.filename))

    # Replace the original output filename with "out.txt"
    with open(os.join(app.config["TESTING_DIR"], "main.cpp"), "r") as f:
        text = f.read()
    of_name = re.search(r"ofstream(.*?)\(").group(1)
    text = re.sub(r"ofstream(.*);", f"ofstream{of_name}(out.txt)", text)
    with open(os.join(app.config["TESTING_DIR"], "main.cpp"), "w") as f:
        f.write(text)
    
    # Create testing files from the database
    test_id = request.form.get("test_id")
    test_files = db_session.query(TestFiles).filter(TestFiles.test_id == test_id).all()
    for test_file in test_files:
        with open(os.join(app.config["TESTING_DIR"], test_file.filename), "w") as f:
            f.write(test_file.file_content)

    # Run the test
    test = db_session.query(Tests).filter(Tests.id == test_id).first()
    # Command: 
    # cd {}; python3 test_maker.py; g++ -std=c++11 Project4_main.cpp; ./a.out python_test.txt; python3 test_verifier.py > log.txt
    # python3 test_maker.py; g++ -std=c++11 Project4_main.cpp; ./a.out python_test.txt; python3 test_verifier.py > log.txt
    test_process = subprocess.run(test.command, shell=True)
    if test_process.returncode != 0:
        return jsonify({"test_result": "Something went wrong, please check if your program can be compiled"})
    with open(os.join(app.config["TESTING_DIR"], "log.txt"), "r") as f:
        test_result = f.read()
        return jsonify({"test_result": test_result})



# Nonblocking Api workflow:
# 1. Create a folder and save all uploads into the folder
# 2. Stores all testing files from the database into the folder
# 3. Pushes the (folder name, sid) tuple into a queue, which is to be handled by a dedicated testing thread
# 4. Testing thread pops a tuple and finishes the corresponding test
# 5. Testing thread emits the test logs to the client with sid=sid
@app.route("/api/project4_submit_test", methods=["POST"])
def submit_test():
    files = request.files.getlist("file")
    test_id = request.form.get("test_id")
    client_id = request.form.get("client_id")
    
    global dir_id
    dir_id = (dir_id + 1)%100
    dirname = f"./test_space/{dir_id}"
    print(dirname)
    subprocess.run(f"mkdir ./test_space/{dir_id}", shell=True)

    for file in files:
        file.save(f"{dirname}/{file.filename}")

    # Replace the original output filename with "out.txt"
    try:
        with open(f"{dirname}/Project4_main.cpp", "r") as f:
            text = f.read()
    except:
        return jsonify({"msg": "You did not submit Project4_main.cpp"}), 206
    of_name = re.search(r"ofstream(.*)\(", text).group(1)
    text = re.sub(r"ofstream(.*);", f"ofstream{of_name}(\"out.txt\");", text)
    with open(f"{dirname}/Project4_main.cpp", "w") as f:
        f.write(text)

    # Create testing files from the database
    test_id = request.form.get("test_id")
    test_files = db_session.query(TestFiles).filter(TestFiles.test_id == test_id).all()
    for test_file in test_files:
        with open(f"{dirname}/{test_file.filename}", "w") as f:
            f.write(test_file.file_content)

    # Push the tuple (sid, dirname, test_id) onto the queue
    lock.acquire()
    test_queue.put((client_id, dirname, test_id))
    lock.release()

    print("task added to queue")

    return jsonify({"msg": "Test is waiting for execution", "client_id": client_id}), 202


@app.route("/api/get_result/<client_id>", methods=["GET"])
def get_result(client_id):
    lock.acquire()
    if client_id in results:
        test_result = results.pop(client_id)
        code = 200
        lock.release()
    else:
        test_result = "Test is yet to complete"
        code = 202
        lock.release()
    return jsonify({"test_result": test_result, "stage_num": stage_num}), code


def tester():
    while(True):
        lock.acquire()
        if test_queue.empty():
            lock.release()
            time.sleep(1)
            continue

        global stage_num
        stage_num = 0
        
        task = test_queue.get() # (client_id, dirname, test_id)
        client_id, dirname, test_id = task
        print(f"task: {task}\n")
        lock.release()

        # run the test
        client_id, dirname, test_id = task
        test = db_session.query(Tests).filter(Tests.id == test_id).first()

        stage_num = 1
        # cd {}; python3 test_maker.py; g++ -std=c++11 Project4_main.cpp; 
        # ./a.out python_test.txt; python3 test_verifier.py > log.txt
        command = test.command.format(dirname)
        # test_process = subprocess.run(command, shell=True)

        
        stage_num = 2
        test_process = subprocess.run(f"python3 test_maker.py", shell=True, cwd=dirname)
        if test_process.returncode != 0:
            lock.acquire()
            results[client_id] = f"died on 2 {test_process.returncode}"
            # results[client_id] = test_process.stdout
            lock.release()
            continue

        stage_num = 3
        test_process = subprocess.run(f"g++ -std=c++11 Project4_main.cpp", shell=True, cwd=dirname)
        if test_process.returncode != 0:
            lock.acquire()
            results[client_id] = "died on 3"
            # results[client_id] = test_process.stdout
            lock.release()
            continue

        stage_num = 4
        test_process = subprocess.run(f"./a.out python_test.txt", shell=True, cwd=dirname)
        if test_process.returncode != 0:
            lock.acquire()
            results[client_id] = "died on 4"
            # results[client_id] = test_process.stdout
            lock.release()
            continue

        stage_num = 5
        test_process = subprocess.run(f"python3 test_verifier.py > log.txt", shell=True, cwd=dirname)
        if test_process.returncode != 0:
            lock.acquire()
            results[client_id] = "died on 5"
            # results[client_id] = test_process.stdout
            lock.release()
            continue

        stage_num = 6
        if test_process.returncode != 0:
            lock.acquire()
            results[client_id] = "Something went wrong, please check if your program can be compiled"
            # results[client_id] = test_process.stdout
            lock.release()
            continue

        with open(f"{dirname}/log.txt", "r") as f:
            lock.acquire()
            stage_num = 7
            test_result = f.read()
            results[client_id] = test_result
            lock.release()


        subprocess.run(f"rm -rf {dirname}", shell=True)

        # lock.acquire()
        # results[client_id] = "Im a retard"
        # lock.release()


if __name__ == "__main__":
    tester_thread = threading.Thread(target=tester)
    tester_thread.start()

    app.run("0.0.0.0", 8080)
    


