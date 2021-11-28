import React from 'react'
import { Link } from 'react-router-dom'
export default function Cocktail({ test_id, test_name }) {
  return (
    <article className='cocktail'>
      {/* <div className='img-container'>
        <img src={image} alt={name} />
      </div> */}
      <div className='cocktail-footer'>
        <h3>{test_name}</h3>
        {/* <h4>{glass}</h4> */}
        {/* <p>{info}</p> */}
        <Link to={`/cocktail/${test_id}`} className='btn btn-primary btn-details'>
          details
        </Link>
      </div>
    </article>
  )
}
