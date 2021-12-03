import React, { useEffect, useState } from 'react'
import Loading from '../components/Loading'
import { useGlobalContext } from '../context'
import { useParams, Link } from 'react-router-dom'

export default function SingleTest() {
    const { testName, testId } = useParams()
    const [randomId, setRandomId] = useState(0)
    const [loading, setLoading] = useState({isLoading: false, message: ""})
    const [submitTest, setSubmitTest] = useState(false)
    const [test, setTest] = useState({
        testId: 0,
        testName: "No Test",
        testResult: "NOT TESTED YET"
    })

    useEffect(() => {
        const newTest = {
            testId: testId,
            testName: testName,
            testResult: "NOT TESTED YET"
        }
        setTest(newTest)

    }, [testId])


    useEffect(() => {
        const periodicFetch = setInterval(() => {
            if(!submitTest){
                return
            }
            fetch(`https://backend-qnhlulg43q-pd.a.run.app/api/get_result/${randomId}`, {
            // fetch(`http://192.168.1.100:8080/api/get_result/${randomId}`, {
                method: "get"
            }).then((res) => {
                if (res.status !== 200) {
                    return;
                }
                res.json().then((data) => {
                    setSubmitTest(false)
                    setLoading({
                        isLoading: false,
                        message: ""
                    })
                    setTest({
                        testId: testId,
                        testName: testName,
                        testResult: data.test_result
                    })
                    clearInterval(periodicFetch)
                })
            }).catch((err) => {
                console.log("Fetch Error :-S", err);
            });
        }, 5000);
      
        return () => clearInterval(periodicFetch);  
    }, [submitTest])

    function handleSubmit(event) {
        event.preventDefault()

        const newId = Math.floor(Math.random() * 10000)
        console.log("newId ", newId)
        setRandomId(newId)
        console.log("randomId ", randomId)

        let myForm = new FormData(event.target)
        myForm.append("test_id", testId)
        myForm.append("client_id", newId)

        setLoading({isLoading: true, message: ""})
        setSubmitTest(true)
        fetch("https://backend-qnhlulg43q-pd.a.run.app/api/project4_submit_test", {
        // fetch("http://192.168.1.100:8080/api/project4_submit_test", {
            body: myForm,
            method: "post"
        }).then((response) => {
            if(response.status != 202){
                throw "Something is wrong"
            }
            return response.json();
        }).then((data) => {
            setLoading({isLoading: true, message: data.msg})
        }).catch((e) => {
            setSubmitTest(false)
            setLoading({isLoading: false, message: ""})
            setTest({
                testId: testId,
                testName: testName,
                testResult: e
            })
        })
    }

    function handleClick() {
        document.getElementById('file').click();
    }
    
    if (loading.isLoading) {
        return <Loading/>
    }

    if (!test) {
        return <h2 className='section-title'>no test to display</h2>
    } else {
        const { testName, testResult } = test
        return (
            <section className='section cocktail-section'>
                <Link to='/' className='btn btn-primary'>
                    back home
                </Link>
                <h2 className='section-title'>{testName}</h2>

                <div className='sumbitForm'>
                    <form onSubmit={handleSubmit} className='flexForm'>
                        <div className='flex-left'> 
                            <div className='btn' onClick={handleClick}>
                                Choose Files
                            </div>
                            <div className='asd'> 
                                <input type="file" id="file" multiple name="file" />
                            </div>
                        </div>
                        <div className='flex-right'>
                            <button type="submit" className="btn">Upload Test Files</button>
                        </div>
                        
                    </form>
                </div>
                
                <div className='testResult'>
                    <p>
                        <p>{testResult}</p>
                    </p>
                </div>
                
            </section>
        )
    }
}