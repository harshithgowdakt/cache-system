const http = require("http"),
	express = require('express'),
	bodyParser = require('body-parser'),
	logger = require("morgan");
	path = require("path")
	userRoutes = require("./routes/user");
	homeRoute = require("./routes/home")

const app = express(), PORT = 8080;

app.use(logger("dev"));
app.use(bodyParser.json());
app.use( express.static((path.join(__dirname, 'public'))))
app.use('/api', userRoutes);

let server = http.createServer(app);
server.listen(PORT, () => {
	console.log(`Server started at port ${PORT}`)
});