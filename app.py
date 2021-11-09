# from flask_restful import Api
from flask import Flask, request, render_template, jsonify
import subprocess
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

        # compile the test
        subprocess.run(["g++", "-std=c++11", "./test_space/test.cpp", "-o", f"./test_space/test{random_id}"])   

        # run the test and send results back to the client
        test = subprocess.run(f"./test_space/test{random_id}", stdout=subprocess.PIPE, text=True)
        return jsonify({"test_result": test.stdout})


if __name__ == "__main__":
    app.run("0.0.0.0", 1453)