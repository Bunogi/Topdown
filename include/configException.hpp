#pragma once

#define CATCH_SETTING_ERRORS(settingsFile) catch(SettingNotFoundException &ex) {\
		std::cerr << "\033[31;1mError\033[0m: Failed to find setting " << ex.getPath() << " in file: " << settingsFile << ".\n"; \
		throw 1; \
	} catch (ParseException &ex) { \
		std::cerr << "\033[31;1mError\033[0m: Syntax error " << ex.getFile() << ": " << ex.getLine() << ": " << ex.getError() << "\n"; \
		throw 1; \
	} catch (FileIOException &ex) { \
		std::cerr << "\033[31;1mError\033[0m: Failed to open file " << settingsFile << "\n"; \
		throw 1; \
	} catch (SettingTypeException &ex) { \
		std::cerr << "\033[31;1mError\033[0m: Wrong setting type for setting " << ex.getPath() << "\n"; \
		throw 1; \
	}
