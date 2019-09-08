#include "vApplication.hpp"
#include "vPhysicalDevice.hpp"
#include "vQueueFamily.hpp"

#include "vLogger.hpp"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <glog/logging.h>

#include <cstdlib>

#include <iostream>

#include <string>
#include <vector>

namespace po = boost::program_options;

namespace
{
std::ostream& operator<<(std::ostream& stream, const std::vector<std::string>& data)
{
	/* clang-format off */
    std::copy(
        std::begin(data), std::end(data),
        std::ostream_iterator<std::string>(stream, ", ")
    );
	/* clang-format on */
	return stream;
}
}  // namespace

int main(int argc, char* argv[])
{
	po::variables_map vm;
	{  // command-line
		po::options_description options("Generic Options");

		/* clang-format off */
        options.add_options()
            ("help,h"     , "Display this help message")
            ("version"    , "Display the version number")
            ("verbosity,v", po::value<int>(&FLAGS_v)->default_value(FLAGS_v),
                "Print more verbose messages at each additional verbosity level.")
            ("scene-file", po::value<std::vector<std::string>>(), "Input scene file");
		/* clang-format on */

		po::store(
			po::command_line_parser(argc, argv)
				.options(options)
				.run(),
			vm);
		po::notify(vm);

		if (vm.count("help"))
		{
			std::cout << options << "\n";
			return 1;
		}
	}

	if (vm.count("scene-file"))
	{
		std::cout << "Input files are: "
				  << vm["scene-file"].as<std::vector<std::string>>() << "\n";
	}

	logging::ScopedLoggingService logging;

	vApplication application;

	auto physicalDevices	  = application.getPhysicalDevices();
	auto finderPhysicalDevice = std::find_if(
		std::begin(physicalDevices), std::end(physicalDevices), [](const vPhysicalDevice& [[maybe_unused]] device) {
			return true;
		});
	CHECK(finderPhysicalDevice != std::end(physicalDevices));

	const vPhysicalDevice& physicalDevice = *finderPhysicalDevice;

	LOG(INFO) << "Selected " << physicalDevice;

	auto queueFamilies	 = physicalDevice.getQueueFamilies();
	auto finderQueueFamily = std::find_if(
		std::begin(queueFamilies), std::end(queueFamilies), [](const vQueueFamily& [[maybe_unused]] device) {
			return true;
		});
	CHECK(finderQueueFamily != std::end(queueFamilies));

	const vQueueFamily& queueFamily = *finderQueueFamily;

	LOG(INFO) << "Selected " << queueFamily;

	return EXIT_SUCCESS;
}
