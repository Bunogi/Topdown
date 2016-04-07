#pragma once

#define CATCH_SETTING_ERRORS catch(SettingNotFoundException &ex) {\
		std::cerr << "Error: Failed to find setting : " << ex.getPath() << " in file " << settingsFile << ".\n"; \
		throw 1; \
	} catch (ParseException &ex) { \
		std::cerr << "Error: Syntax error : " << ex.getFile() << ": " << ex.getLine() << " : " << ex.getError() << "\n"; \
		throw 1; \
	}
