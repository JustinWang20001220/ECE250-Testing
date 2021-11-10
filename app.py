# from flask_restful import Api
from flask import Flask, request, render_template, jsonify
import subprocess
import re, fileinput
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
    return render_template("./templates/online_tester.html")

@app.route("/api/run_test", methods=["POST"])
def run_test():
    if request.method == "POST":
        h_file = request.files["file"]
        random_id = request.form["random_id"]
        with open(f"./submissions/{random_id}.h", "w", encoding="utf-8") as f:
            f.write(h_file)

        # copy uploaded file into testing environment
        subprocess.run(["cp", f"./submissions/{random_id}.h", "./test_space/Search_tree.h"])
        publicate("./test_space/Search_tree.h")
        
        # compile the test
        subprocess.run(["g++", "-std=c++11", "./test_space/test.cpp", "-o", f"./test_space/test{random_id}"])   

        # run the test and send results back to the client
        test = subprocess.run(f"./test_space/test{random_id}", stdout=subprocess.PIPE, text=True)
        return jsonify({"test_result": test.stdout})


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

