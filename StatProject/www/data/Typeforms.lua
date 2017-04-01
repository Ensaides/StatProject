local body;
if (options) then
	local Typeform = StatProject.GetTypeformData(options[1]);
	local Field = StatProject.GetTypeformField

	if (Typeform) then
		utils.log("Found typeform");
		body = Typeform;
	else
		utils.log("No typeform");
		--body = StatProject.GetTypeforms();
	end
else
	body = StatProject.GetTypeforms();
end

res:setHeader("Content-Type", "application/json");
res:setHeader("Content-Length", #body);
res:write(body);
res:finish();