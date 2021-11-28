import React from 'react'
import Cocktail from './Cocktail'
import Loading from './Loading'
import { useGlobalContext } from '../context'

export default function CocktailList() {
  const { tests, loading } = useGlobalContext()
  if (loading) {
    return <Loading/>
  }
  if (tests.length < 1) {
    return (
      <h2 className='section-title'>
        no tests matched your search criteria
      </h2>
    )
  }
  return (
    <section className='section'>
      <h2 className='section-title'>tests</h2>
      <div className='cocktails-center'>
        {tests.map((item) => {
          return <Cocktail key={item.test_id} {...item} />
        })}
      </div>
    </section>
  )
}
