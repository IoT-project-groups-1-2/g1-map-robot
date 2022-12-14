'use strict'

const express = require('express')
const path = require('path')
const app = express()
app.use(express.static(path.join(__dirname, '/')));

app.get('/', (req, res)=>{
    res.sendFile(path.join(__dirname + '/drive.html'));
})

app.get('/drive', (req, res)=>{
    res.sendFile(path.join(__dirname + '/drive.html'));
})

app.get('/map', (req, res)=>{
    res.sendFile(path.join(__dirname + '/map.html'));
})

app.listen(3000, () => console.log('Server listening on port 3000'));