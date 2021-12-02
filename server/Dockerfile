FROM python:3.9

RUN pip3 install flask flask-cors sqlalchemy pymysql cloud-sql-python-connector

COPY src/ app/
WORKDIR /app

ENV PORT 8080

CMD ["python3", "app.py"]