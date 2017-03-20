local body = StatProject.GetTypeforms();
res:setHeader("Content-Type", "application/json");
res:setHeader("Content-Length", #body);
res:write(body);
res:finish();