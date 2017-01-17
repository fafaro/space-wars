#pragma once

namespace FS { namespace FGame
{
	class KeyScanner
	{
	public:
		class IListener
		{
			friend class KeyScanner;
		protected:
			virtual void onKeySequence(const char *code, int id) = 0;
		};

	private:
		struct CodeEntry
		{
			const char *code;
			IListener *listener;
			int id;
			int currPos; // points to next char to identify (starts at 0 for none identified)
		};

		typedef std::vector<CodeEntry> CodeEntryList;

	private:
		CodeEntryList m_codeList;

	public:
		KeyScanner();

		void addCode(const char *code, IListener *listener, int id);
		void inputKey(char c);
	};
}}