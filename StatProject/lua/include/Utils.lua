utils	= utils or require("utils");
fs		= fs or require("fs");

local timedate = os.date("%x", os.time()) .. os.date("_%X", os.time());
timedate = string.gsub(timedate, "/", ".");

local log = fs.openSync("../../logs/serverlog" .. timedate .. ".txt", "a");


utils.log = function(String, printdate, logtofile)
	-- By default, print the date
	if (printdate == nil) then
		printdate = true;
	end

	if (logtofile == nil) then
		logtofile = true;
	end

	local printstring = "";

	if (printdate) then
		if (params["colorize"] ~= false) then
			printstring = utils.colorize("property", os.date("%x", os.time())) .. utils.colorize("braces", os.date(" %X", os.time())) .. " ";
		else
			printstring = os.date("%x", os.time()) .. os.date(" %X", os.time()) .. " ";
		end
	end

	printstring = printstring .. String;

	utils.print(printstring);

	if (logtofile) then
		utils.logtofile(utils.strip(printstring));
	end
end

utils.logtofile = function(String)
	if (log ~= nil) then
		fs.write(log, 0, String);
		fs.write(log, 0, "\n");
	else
		if (params["colorize"] ~= false) then
			utils.log(utils.colorize("err", "ERROR: Unable to write to log file!"), true, false);
		else
			utils.log("ERROR: Unable to write to log file!", true, false);
		end
	end
end

utils.string = utils.string or {};
utils.string.ends = function(String, End)
	return End == '' or string.sub(String, -string.len(End)) == End;
end