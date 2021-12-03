<h1>Deployment</h1>
## Backend<br/>
1. cd server <br/>
2. gcloud builds submit --tag gcr.io/ece250-testing-server/backend <br/>
3. gcloud run deploy --image gcr.io/ece250-testing-server/backend <br/>
<br/>
## Frontend<br/>
1. npm run build<br/>
2. firebase init hosting (with "build" as the public folder)<br/>
3. firebase deploy<br/>
<br/>
## Database<br/>
For database, make sure collate of sql file is "utf8mb4_general_ci"