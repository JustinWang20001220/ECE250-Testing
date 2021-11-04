from flask_restful import Api
from flask import Flask
from flask_cors import CORS
from flask_session import Session
from routes import Upload, Report

class FlaskApp:
    def __init__(self):
        self.app = Flask(__name__)
        CORS(self.app)
        Session(self.app)
        self.api = Api(self.app, prefix="/api/v0.0")
    
    def runserver(self, **kwargs):
        self.app.run(**kwargs)

    def register_route(self):
        self.api.add_resource(Upload, "/upload")
        self.api.add_resource(Report, "/report")