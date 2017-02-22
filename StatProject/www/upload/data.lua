querystring = querystring or require("querystring");

local function split(str, delimiter)
	local result = { }
	local from  = 1
	local delim_from, delim_to = string.find( str, delimiter, from  )
	while delim_from do
	table.insert( result, string.sub( str, from , delim_from-1 ) )
	from  = delim_to + 1
	delim_from, delim_to = string.find( str, delimiter, from  )
	end
	table.insert( result, string.sub( str, from  ) )
	return result
end

local chunks = {}
local length = 0

req:on('data', function (chunk, len)
	length = length + 1
	chunks[length] = chunk
end)

req:on('end', function ()
	local body = table.concat(chunks, "")
	body = "Upload complete!";
	res:writeHead(200, {
		["Content-Type"] = "text/plain",
		["Content-Length"] = #body
	})
	res:finish(body)

	if (req["headers"]["Content-Type"]) then
		local boundary = string.match(req["headers"]["Content-Type"], "%sboundary=(..+)");

		-- Split the multipart message in to individual contents
		local contents = split(table.concat(chunks, ""), boundary);
		for _, v in pairs(contents) do
			local ContentType;
			local ContentDisposition;
			local filename;
			local name;
			
			-- Split it once again by line ends
			local lines = split(v, "\r\n");
			for _, line in pairs(lines) do
				if (querystring.parse(line, ";", ":")["Content-Type"]) then
					ContentType = querystring.parse(line, ";", ":")["Content-Type"];

				elseif (querystring.parse(line, ";", ":")["Content-Disposition"]) then
					local ContentDispositionInfo = querystring.parse(line, ";", ":");
					ContentDisposition = ContentDispositionInfo["Content-Disposition"];

					ContentDispositionInfo["Content-Disposition"] = nil;
					for k,_ in pairs(ContentDispositionInfo) do
						if (querystring.parse(k, ";", "=")[" filename"]) then
							filename = querystring.parse(k, ";", "=")[" filename"]:gsub('"', '');
						elseif (querystring.parse(k, ";", "=")[" name"]) then
							name = querystring.parse(k, ";", "=")[" name"]:gsub('"', '');
						end
					end
				end
			end

			if (ContentType and ContentDisposition and filename and name) then
				local strcpy = v;
				for i=1, 4, 1 do
					local _, index = string.find(strcpy, "\r\n");
					strcpy = string.sub(strcpy, index + 1, -1);
				end

				strcpy = string.sub(strcpy, 1, -3);

				local file = fs.openSync("../../data/upload/" .. filename, "w");
				if (file) then
					utils.log("Saving file: " .. "data/upload/" .. filename)
					fs.writeSync(file, 0, strcpy);
				end
			end
		end
	end
end)