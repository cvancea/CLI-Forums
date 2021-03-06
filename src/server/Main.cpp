#include <server/Server.h>
#include <common/Logger.h>

#include <unistd.h>


int main(int argc, char *argv[])
{
#ifdef ENABLE_LOGGING
    Logger::GetInstance().SetOutputToFile(true, Logger::RULE_ALL, fmt::format("serverdbg_{}", getpid()));
    Logger::GetInstance().SetOutputToStdout(true, Logger::RULE_ALL);
#else
    Logger::GetInstance().SetOutputToStdout(true, Logger::RULE_MESSAGE | Logger::RULE_ERROR);
#endif

    ECode ret;
    Peer server_params;

	if (argc < 2) {
        LOG_ERROR("Usage: {} <port>", argv[0]);
        return EXIT_FAILURE;
	}

    server_params.client_id = "SERVER";
    server_params.ip = INADDR_ANY;
    server_params.port = atoi(argv[1]);

    if (!server_params.IsValid()) {
        LOG_ERROR("Invalid parameters specified.");
        return EXIT_FAILURE;
    }


    Server server(server_params);
    ret = server.Init();
    if (ret != ECode::OK) {
        LOG_ERROR("Can't init the server, errcode: {}", ret);
        return EXIT_FAILURE;
    }

    ret = server.Run();
    if (ret != ECode::OK) {
        LOG_ERROR("An error occured while running the server, errcode: {}", ret);
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
