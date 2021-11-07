from flask_restful import Resource
from flask import request
import subprocess

TESTS = ["push", "rebalancing", ...]

class Upload(Resource):
    # gets the uploaded binary file and stores it in a folder (could use database later)
    def post(self):
        h_file = request.files["file"]
        username = request.form["username"]
        with open(f"./submissions/{username}.h", "w", encoding="utf-8") as f:
            f.write(h_file)
        return 200


class Report(Resource):
    def run_test(test_number: int, username: str) -> bool:
        
        
        # compile the code with the specified test and run it
        subprocess.run(f"cp ./submissions/{username}.h ./test_space/Search_tree.h")

        subprocess.run(f"g++ -std=c++11 test{test_number}.cpp -o {username}_test{test_number}")
        result = subprocess.run(f"./test_space/{username}_test{test_number}.out")
        return True if result.returncode is 0 else False
    
    # sends the report from the unit tests back to the front end
    def get(self):
        username = request.form["username"]
        
        results: dict = {}
        for i in range(len(TESTS)):
            results[TESTS[i]] = self.run_test(i, username)
        return results, 200