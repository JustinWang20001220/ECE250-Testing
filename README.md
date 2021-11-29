<h1> TODO List </h1>
<h2> Front End </h2>
1. online_tester.html: <br/>

    1. Maybe render the html with description of the tests
    2. The webpage should be able to make request to the run_test api and display the results on the same page
2. Front-End Server: React<br/>
3. Socketio with React: client needs to connect to server before submitting each test, then disconnect when test is completed<br/>
3. Dynamic graphical representation of the structure of tree formed by user's code<br/>

<h2> Back End </h2>
1. multiple-file submission ✅<br/>
2. MySQL database for storing tests ✅<br/>
3. Api: live searching for tests ✅<br/>
4. Api: User download test file <br/>
5. Make run_selected() an async api, returns 202 when the api is already in use <br/>
6. run_selected(): use makefile to allow arbitrary filename submission<br/>
7. Api: user uploading tests onto the database<br/>
8. Api: login system--website stores the uploads from each user<br/>
9. Api: tests rating--users can rate tests after running them<br/>
10. graphical debugger
