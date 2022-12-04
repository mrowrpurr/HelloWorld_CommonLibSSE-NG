std::ofstream loggerOutputForStdoutRedirection;

void RedirectStdoutToFile() {
	auto stdout_path = SKSE::log::log_directory();
	*stdout_path /= fmt::format("{}.STDOUT.log", SKSE::PluginDeclaration::GetSingleton()->GetName());
	loggerOutputForStdoutRedirection = std::ofstream{stdout_path->string()};
	std::cout.rdbuf(loggerOutputForStdoutRedirection.rdbuf());
}

void InitializeLog() {
	auto path = SKSE::log::log_directory();
	if (!path) SKSE::stl::report_and_fail("Failed to find standard logging directory"sv);
	*path /= fmt::format("{}.log", SKSE::PluginDeclaration::GetSingleton()->GetName());
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
	const auto level = spdlog::level::trace;
	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
	log->set_level(level);
	log->flush_on(level);
	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
}

SKSEPluginLoad(const SKSE::LoadInterface* skse) {
	InitializeLog();
	SKSE::log::info("Hello, world!");

	RedirectStdoutToFile();
	std::cout << "Hello, STDOUT!" << std::endl;

	SKSE::Init(skse);

	// Once all plugins and mods are loaded, then the ~ console is ready and can be printed to
	SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message){
		if (message->type == SKSE::MessagingInterface::kDataLoaded) {
			RE::ConsoleLog::GetSingleton()->Print("Hello, world! kDataLoaded");
			SKSE::log::info("kDataLoaded");
			std::cout << "kDataLoaded" << std::endl;
		}
			
	});

	return true;
}