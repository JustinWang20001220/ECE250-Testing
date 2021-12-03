<h1>Deployment</h1>
Backend<br/>
```bash 
cd server 
gcloud builds submit --tag gcr.io/ece250-testing-server/backend 
gcloud run deploy --image gcr.io/ece250-testing-server/backend
```
<br/>
Frontend<br/>
```bash
npm run build
firebase init hosting
firebase deploy
```
<br/>
For database, make sure collate of sql file is "utf8mb4_general_ci"