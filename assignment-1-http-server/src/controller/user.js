const fs = require("fs"),
	usersData = JSON.parse(fs.readFileSync(__dirname + '/../db/users.json', 'utf8'));

module.exports.getUserById = async function (userId) {
	for (let i = 0; i < usersData.users.length; i++) {
		if (usersData.users[i].id == userId) {
			return usersData.users[i];
		}
	}
	return false;
}

module.exports.saveUser = async function (user) {
	for (let i = 0; i < usersData.users.length; i++) {
		if (usersData.users[i].id == user.id) {
			return false;
		}
	}
	usersData.users.push(user);
	fs.writeFileSync(__dirname + '/../db/users.json', JSON.stringify(usersData), 'utf-8');
	return true;
}