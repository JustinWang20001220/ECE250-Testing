import React, { useState, useContext, useEffect } from 'react'
import { useCallback } from 'react'

const url = 'https://backend-qnhlulg43q-pd.a.run.app/api/search_test/'
// const url = "http://192.168.1.100:8080/api/search_test/"
const AppContext = React.createContext()

const AppProvider = ({ children }) => {
  const [loading, setLoading] = useState(true)
  const [searchTerm, setSearchTerm] = useState('test')
  const [tests, setTests] = useState([])

  const fetchTests = useCallback( async () => {
    setLoading(true)
    try {
      const api = `${url}${searchTerm}`
      // console.log(api)
      const response = await fetch(`${url}${searchTerm}`)
      const data = await response.json()
      // console.log(data);
      const { new_tests } = data

      if (new_tests) {
        setTests(new_tests)
      } else {
        setTests([])
      }
      
      setLoading(false)
    } catch (error) {
      console.log(error)
      setLoading(false)
    }
  },[searchTerm])

  useEffect(() => {
    fetchTests()
  }, [searchTerm, fetchTests])

  return (
    <AppContext.Provider value={{ loading, tests, searchTerm, setSearchTerm }}>
      {children}
    </AppContext.Provider>
  )
}
// make sure use
export const useGlobalContext = () => {
  return useContext(AppContext)
}

export { AppContext, AppProvider }