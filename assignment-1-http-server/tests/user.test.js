const expect = require("chai").expect,
	userController = require("./../src/controller/user"),
	testData = require("./testData/userTestData");

describe("Testing the User Controller", function () {

	it("Should return true, if user is saved successfully",async function () {
		const result = await userController.saveUser(testData.saveUserReq);
		expect(result).to.equal(testData.saveUserRes.res);
	});

	it("Should return the user name",async function () {
		const result = await userController.getUserById(testData.getUserReq.id)
		expect(result.name).to.equal(testData.getUserRes.name);
	})
});