const express = require("express"),
    userController = require("../controller/user"),
    router = express.Router();

router.get('/users/:id', async (req, res, next) => {
    let user = await userController.getUserById(req.params.id);

    if (user) return res.status(200).send(user);
    return res.status(400).send("User not found");
});

router.post('/users', async (req, res, next) => {
    let reslut = await userController.saveUser(req.body);

    if (reslut) {
        return res.status(200).send("OK")
    } else {
        return res.status(400).send("User id already exists");
    }
});

module.exports = router;