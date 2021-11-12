<h1>Deployment with Google Cloud</h1>
From the deployment branch<br/>
```bash cd server gcloud builds submit --tag gcr.io/ece250-testing-server/ece250-testing-deploy gcloud run deploy --image gcr.io/ece250-testing-server/ece250-testing-deploy ```