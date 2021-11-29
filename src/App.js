import React from "react";
import { BrowserRouter as Router, Route, Switch } from "react-router-dom";
// import pages
import Home from "./pages/Home";
import About from "./pages/About";
// import SingleCocktail from "./pages/SingleCocktail";
import SingleTest from "./pages/SingleTest";
import Error from "./pages/Error";
// import components
import Navbar from "./components/Navbar";

import io from "socket.io-client"

let sid = null
const socket = io.connect("/")

socket.on("connected", (data) => {
  sid = data.sid
  console.log(`sid: ${sid}`)
})

function App() {
  return (
    <Router>
      <Navbar />
      <Switch>
        <Route exact path="/">
          <Home />
        </Route>
        <Route path="/about">
          <About />
        </Route>
        {/* <Route path="/cocktail/:testId">
          <SingleCocktail socket={socket} />
        </Route> */}
        <Route path="/test/:testName/:testId">
          <SingleTest socket={socket} sid={sid} />
        </Route>
        <Route path="*">
          <Error />
        </Route>
      </Switch>
    </Router>
  );
}

export default App;
