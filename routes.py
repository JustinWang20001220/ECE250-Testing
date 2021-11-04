from flask_restful import Resource
from flask import request, session

TESTS = ["push", "rebalancing", ...]

class Upload(Resource):
    # gets the uploaded binary file and stores it in a folder (could use database later)
    def post(self):
        h_file = request.files["file"]
        with open(f"./submissions/{session.username}.h", "w", encoding="utf-8") as f:
            f.write(h_file)


class Report(Resource):
    def run_test(test_number: int) -> bool:
        # compile the code with the specified test and run it
        pass
    
    # sends the report from the unit tests back to the front end
    def get(self):
        results: dict = {}
        for i in range(len(TESTS)):
            results[TESTS[i]] = self.run_test(i)
        return results, 200