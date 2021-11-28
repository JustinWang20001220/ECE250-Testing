import React, { useState, useContext, useEffect } from 'react'
import { useCallback } from 'react'

const url = 'https://www.thecocktaildb.com/api/json/v1/1/search.php?s='
const AppContext = React.createContext()

const AppProvider = ({ children }) => {
  const [loading, setLoading] = useState(true)
  const [searchTerm, setSearchTerm] = useState('a')
  const [tests, setTests] = useState([])

  const fetchTests = useCallback( async () => {
    setLoading(true)
    try {
      const response = await fetch(`${url}${searchTerm}`)
      const data = await response.json()
      console.log(data);
      const { new_tests } = data
      // if (tests) {
      //   const newCocktails = tests.map((item) => {
      //     const {
      //       idDrink,
      //       strDrink,
      //       strDrinkThumb,
      //       strAlcoholic,
      //       strGlass,
      //     } = item

      //     return {
      //       id: idDrink,
      //       name: strDrink,
      //       image: strDrinkThumb,
      //       info: strAlcoholic,
      //       glass: strGlass,
      //     }
      //   })
      //   setCocktails(newCocktails)
      // } else {
      //   setCocktails([])
      // }
      setTests(new_tests)
      setLoading(false)
    } catch (error) {
      console.log(error)
      setLoading(false)
    }
  },[searchTerm])
  useEffect(() => {
    fetchTests()
  }, [searchTerm,fetchTests])
  return (
    <AppContext.Provider
      value={{ loading, tests, searchTerm, setSearchTerm }}
    >
      {children}
    </AppContext.Provider>
  )
}
// make sure use
export const useGlobalContext = () => {
  return useContext(AppContext)
}

export { AppContext, AppProvider }
