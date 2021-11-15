# from flask_restful import Api
from flask import Flask, request, render_template, jsonify, make_response
import subprocess
import re, os
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


def publicate(file_path):
    file = open(file_path, "r")
    text = file.read()
    file.close()

    text = re.sub("private", "public", text)

    file = open(file_path, "w")
    file.write(text)
    file.close()



if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=int(os.environ.get("PORT", 8080)))

