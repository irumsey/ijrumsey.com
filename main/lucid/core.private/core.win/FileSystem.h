#pragma once

#ifndef CORE_PRIVATE
#	error core layer violation
#endif

#include <list>
#include <map>
#include <lucid/core/FileSystem.h>
#include <lucid/core.private/core.win/Defines.h>

LUCID_CORE_WIN_BEGIN

///	FileSystem
///
///
class FileSystem : public LUCID_CORE::FileSystem
{
public:
	virtual ~FileSystem() = default;

	virtual void addListener(std::string const &path, Listener *listener) override;

	virtual void removeListener(Listener *listener) override;

	virtual void scan() override;

	static FileSystem &instance();

protected:
	FileSystem() = default;

private:
	std::map<FileInfo, std::list<Listener*>> _listeners;

	LUCID_PREVENT_COPY(FileSystem);
	LUCID_PREVENT_ASSIGNMENT(FileSystem);
};

LUCID_CORE_WIN_END