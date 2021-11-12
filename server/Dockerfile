FROM python:3.9

RUN pip3 install flask

COPY src/ app/
WORKDIR /app

ENV PORT 8080

CMD exec python3 app.py