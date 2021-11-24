# from flask_restful import Api
import os
from flask import Flask, request, render_template, jsonify, make_response
import subprocess
import re, fileinput

from sqlalchemy.orm.session import sessionmaker
from sqlalchemy import text
from database import Base, engine, Projects, Tests, TestFiles
# from flask_cors import CORS
# from flask_session import Session
# from routes import Upload, Report

# class FlaskApp:
#     def __init__(self):
#         self.app = Flask(__name__)
#         # CORS(self.app)
#         # Session(self.app)
#         self.api = Api(self.app, prefix="/api/v0.0")
    
#     def runserver(self, **kwargs):
#         self.app.run(**kwargs)

#     def register_route(self):
#         self.api.add_resource(Upload, "/upload")
#         self.api.add_resource(Report, "/report")


app = Flask(__name__)
app.config["TESTING_DIR"] = "./test_space"

Base.metadata.create_all(engine)
DB_Session = sessionmaker(bind=engine)
db_session = DB_Session()


@app.route("/")
def index():
    # Maybe render the html with description of the tests.
    # The webpage should be able to make request to the run_test api
    # and display the results on the same page
    return render_template("online_tester.html")

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

        # test_result = test.stdout
        # results = test_result.split("\n")

        # delete file
        subprocess.run(["rm", "-f", f"./submissions/{random_id}.h", f"./test_space/test{random_id}", "./test_space/Search_tree.h"])

        return jsonify({"test_result": result, "segmentation_fault": is_faulty})


# # Api that returns the available project id's
# @app.route("api/get_projects", method=["GET"])
# def get_projects():
#     pass


# Api that takes in project_id and returns the tests (name + id)
@app.route("/api/get_all_tests", method=["GET"])
def get_all_tests():
    pass

# Api: live search for test names
@app.route("api/search_test", method=["POST", "GET"])
def search_test():
    input = request.form.get("text")
    similar_tests = db_session.query(Tests).filter(Tests.test_name.ilike("%" + input + "%")).all()

    # sql = f"SELECT * FROM tests WHERE test_name LIKE '%{input}%'"
    # similar_tests = db_session.execute(text(sql)).fetchall()

    return jsonify([{test.test_name : test.id} for test in similar_tests])



# Api that takes one parameters: test_id
# input form: 
@app.route("/api/run_selected", method=["POST"])
def run_selected():
    files = request.files.getlist("file")
    for file in files:
        file.save(os.path.join(app.config["TESTING_DIR"], file.filename))
    
    # Create testing files from the database
    test_id = request.form.get("test_id")
    test_files = db_session.query(TestFiles).filter(TestFiles.test_id == test_id).all()
    for test_file in test_files:
        with open(os.join(app.config["TESTING_DIR"], test_file.filename), "w") as f:
            f.write(test_file.file_content)

    # Run the test
    test = db_session.query(Tests).filter(Tests.id == test_id).first()
    subprocess.run(eval(test.command)) 

def publicate(file_path):
    file = open(file_path, "r")
    text = file.read()
    file.close()

    text = re.sub("private", "public", text)

    file = open(file_path, "w")
    file.write(text)
    file.close()



if __name__ == "__main__":
    app.run("0.0.0.0", 1453)

