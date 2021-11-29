import React, { useEffect, useState } from 'react'
import Loading from '../components/Loading'
import { useGlobalContext } from '../context'
import { useParams, Link } from 'react-router-dom'


export default function SingleTest({socket, sid}) {
    const {testId} = useParams()
    const [loading, setLoading] = useState(null)
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
            testName: this.props.location.testName,
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
        return <Loading message={loading.message}/>
    }
    //asdf

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
                <p>
                    <span className='drink-data'>Test Result :</span> {testResult}
                </p>
            </section>
        )
    }
}