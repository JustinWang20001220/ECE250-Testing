import React from 'react'
import { Link } from 'react-router-dom'


export default function Cocktail({ test_id, test_name }) {
  const toSingleTest = {
    pathname: `/cocktail/${test_id}`,
    testName: test_name
  }
  
  return (
    <article className='cocktail'>
      {/* <div className='img-container'>
        <img src={image} alt={name} />
      </div> */}
      <div className='cocktail-footer'>
        <h3>{test_name}</h3>
        {/* <h4>{glass}</h4> */}
        {/* <p>{info}</p> */}
        <Link to={toSingleTest} className='btn btn-primary btn-details'>
          Enter
        </Link>
      </div>
    </article>
  )
}
