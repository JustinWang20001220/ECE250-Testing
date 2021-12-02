from sqlalchemy import create_engine, Column, String, Integer, Text
from sqlalchemy.ext.declarative import declarative_base
from google.cloud.sql.connector import connector
import pymysql

def getconn():
    conn = connector.connect(
        "ece250-testing-server:northamerica-northeast2:online-tester", 
        "pymysql",
        user="root",
        password="1qaz2wsx",
        db="online-tester"
    )
    return conn

engine = create_engine("mysql+pymysql://", creator=getconn)

# engine = create_engine("mysql+pymysql://root:1qaz2wsx@localhost:3306/online-tester")
Base = declarative_base()


class Projects(Base):
    __tablename__ = "projects"
    id = Column(Integer, primary_key=True)            # project id
    name = Column(String(32))       # The project name. Eg. ECE250-Project4
    type = Column(String(32))       # "data structure" or "program"
    test_ids = Column(String(32))   # The list of test id's for this project


class Tests(Base):
    __tablename__ = "tests"
    id = Column(Integer, primary_key=True)
    test_name = Column(String(32))          # <Course>-<Project>-<Test>. Eg. ECE250-Project4-BasicInputText
                                            #                            Eg. ECE250-Project4-DAGDSTest

    project_id = Column(Integer)
    file_ids = Column(String(32))
    command = Column(String(100))           # The input list for subprocess.run(), stored as a string


class TestFiles(Base):
    __tablename__ = "test_files"
    id = Column(Integer, primary_key=True)
    test_id = Column(Integer)
    filename = Column(String(32))
    file_content = Column(Text)

# Base.metadata.drop_all(engine)
# Base.metadata.create_all(engine)