<h1>Deployment</h1>
Backend<br/>
 - cd server <br/>
`gcloud builds submit --tag gcr.io/ece250-testing-server/backend` <br/>
`gcloud run deploy --image gcr.io/ece250-testing-server/backend` <br/>
<br/>
Frontend<br/>
`npm run build`<br/>
`firebase init hosting` with "build" as the public folder<br/>
`firebase deploy`<br/>
<br/>
For database, make sure collate of sql file is "utf8mb4_general_ci"