#include "StatProjectLua.h"
#include <iostream>
#include "IO.h"

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120

#include <CL/cl2.hpp>

using namespace std;

static void PrintOpenCLDiagnostics(const cl::Platform& Platform, const std::vector<cl::Device>& Devices);
static string GetDeviceTypeName(cl_device_type DeviceType);

lua_function(OpenCLTest)
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	cl::Platform plat;
	for (auto& p : platforms) 
	{
		IO::Print("Platform Name:\t" + p.getInfo<CL_PLATFORM_NAME>());
		IO::Print("Platform Profile:\t" + p.getInfo<CL_PLATFORM_PROFILE>());
		IO::Print("Platform Vendor:\t" + p.getInfo<CL_PLATFORM_VENDOR>());
		IO::Print("Platform Extensions:\t" + p.getInfo<CL_PLATFORM_EXTENSIONS>());
		cout << endl;

		string platver = p.getInfo<CL_PLATFORM_VERSION>();
		if (platver.find("OpenCL 1.2") != std::string::npos) {
			plat = p;
		}
	}

	if (plat() == NULL) {
		IO::Print("No OpenCL 1.2 platform found.");
		return 0;
	}

	std::vector<cl::Device> Devices;

	plat.getDevices(CL_DEVICE_TYPE_ALL, &Devices);

	PrintOpenCLDiagnostics(plat, Devices);

	cl::Context Context(Devices);
	cl::Program::Sources Sources;

	vector<string> Files;
	IO::GetFilesInDirectory(IO::GetProgramDirectory() + IO::SourceDir + "OpenCL/", Files);

	for (auto& File : Files)
	{
		try
		{
			Sources.push_back(IO::ReadFile(IO::SourceDir + "OpenCL/" + File));
		}
		catch (runtime_error& e)
		{
			IO::Print(e.what());
			throw;
		}
	}

	cl::CommandQueue Queue;

	cl::Program Program(Context, Sources);

	// Compile the program
	try
	{
		Program.build(Devices);
	}
	catch (cl::Error& Exception)
	{
		IO::Print("OpenCL Exception: " + string(Exception.what()));

		for (auto& Device : Devices)
		{
			IO::Print("Build Status:\t"		+ Program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(Device));
			IO::Print("Build Options:\t"	+ Program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(Device));
			IO::Print("Build Log:\t "		+ Program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(Device));
		}

		return 0;
	}

	// Get the kernel from the compiled program
	cl::Kernel Kernel;
	try 
	{
		Kernel = cl::Kernel(Program, "hello");
	}
	catch (cl::Error& er) 
	{
		IO::Print("OpenCL Exception: " + string(er.what()));
		return 0;
	}

	try
	{
		char Arg1[1024];
		Kernel.setArg(0, Arg1);

		Queue.enqueueNDRangeKernel(Kernel, cl::NullRange, cl::NDRange(0), cl::NullRange);
		Queue.finish();

		IO::Print(string(Arg1));
	}
	catch (cl::Error& E)
	{
		IO::Print(to_string(E.err()));
		IO::Print(E.what());
	}

	return 0;
}

static void PrintOpenCLDiagnostics(const cl::Platform& Platform, const std::vector<cl::Device>& Devices)
{
	for (auto& Device : Devices)
	{
		IO::Print("Device Name:\t\t\t"			+ Device.getInfo<CL_DEVICE_NAME>());
		IO::Print("Device Type:\t\t\t"			+ GetDeviceTypeName(Device.getInfo<CL_DEVICE_TYPE>()));
		IO::Print("Device Global Mem Size:\t"	+ to_string(Device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() / (cl_ulong)1e+6) + "mb");
		IO::Print("Device OpenCL C Version:\t"	+ Device.getInfo<CL_DEVICE_OPENCL_C_VERSION>());
		IO::Print("Device Version:\t\t"			+ Device.getInfo<CL_DEVICE_VERSION>());

		for (int i = 0; i < Device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>().size(); i++)
		{
			IO::Print("Device Max Work Items Dimension " + to_string(i + 1) + ":\t" + to_string(Device.getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>()[i]));
		}

		cout << endl;
	}
}

static string GetDeviceTypeName(cl_device_type DeviceType)
{
	switch (DeviceType)
	{
	case CL_DEVICE_TYPE_DEFAULT:
		return "Default";
		break;

	case CL_DEVICE_TYPE_CPU:
		return "CPU";
		break;

	case CL_DEVICE_TYPE_GPU:
		return "GPU";
		break;

	case CL_DEVICE_TYPE_ACCELERATOR:
		return "Accelerator";
		break;

	case CL_DEVICE_TYPE_CUSTOM:
		return "Custom";
		break;

	default:
		return "";
		break;
	}
}