import React, { useEffect, useState } from 'react'
import Loading from '../components/Loading'
import { useGlobalContext } from '../context'
import { useParams, Link } from 'react-router-dom'


export default function SingleTest({socket, sid}) {
    const {testName, testId} = useParams()
    const [loading, setLoading] = useState({isLoading: false, message: ""})
    const [test, setTest] = useState({
        testId: 0,
        testName: "No Test",
        testResult: "none"
    })

    useEffect(() => {
        let testResult = "none"
        socket.on("completed", (data) => {
            testResult = data.test_result
            setLoading({isLoading: false, message: ""})
        })
        
        const newTest = {
            testId: testId,
            testName: testName,
            testResult: testResult
        }
        setTest(newTest)

    }, [testId, socket])

    // TODO
    function onSubmit(test_id) {
        if (document.getElementById("h_file").value === "") {
            return
        }

        let myForm = new FormData(document.getElementById("upload-form"))
        myForm.append("test_id", test_id)
        myForm.append("sid", sid)

        fetch("/api/run_test", {
            body: myForm,
            method: "post"
        }).then((response) => {
            return response.json();
        }).then((data) => {
            setLoading({isLoading: true, message: data.msg})
        })
    }
    
    if (loading.isLoading) {
        return <Loading/>
    }

    if (!test) {
        return <h2 className='section-title'>no test to display</h2>
    } else {
        const { testName, testResult } = test
        return (
            <section className='setction cocktail-section'>
                <Link to='/' className='btn btn-primary'>
                    back home
                </Link>
                <h2 className='section-title'>{testName}</h2>

                {/* Form to submit files */}
                <form enctype="multipart/form-data" id="upload-form" name="upload-form"> 
                    <div>
                        <label for="h_file"></label>
                        <input type="file" id="h_file" name="h_file" accept=".h" multiple/>
                    </div>
                    <button type="button" onclick="onSubmit()">Submit</button>
                </form>


                <p>
                    <span className='drink-data'>Test Result :</span> {testResult}
                </p>
            </section>
        )
    }
}