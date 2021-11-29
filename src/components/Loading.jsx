import React from 'react'

const Loading = (message) => {
 return (
  <div className="loader">
      <p>
          <span>{message}</span>
      </p>
  </div>
 )
}

export default Loading
