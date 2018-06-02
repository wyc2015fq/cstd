#ifndef TTF_HEADER_INCLUDED
#define TTF_HEADER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nana/charset.inl"

typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

int fread(FILE* pf, void* buf, int size) {
  return fread(buf, size, 1, pf);
}

class truetype
{
	struct tt_offset_table
	{
		uint16_t major_version;
		uint16_t minor_version;
		uint16_t num_of_tables;
		uint16_t search_range;
		uint16_t entry_selector;
		uint16_t range_shift;
	};

	struct tt_table_directory
	{
		char	name[4];		//table name
		uint32_t checksum;	//Check sum
		uint32_t offset;	//Offset from beginning of file
		uint32_t length;	//length of the table in bytes
	};

	struct tt_name_table_header
	{
		uint16_t format_selector;		//format selector. Always 0
		uint16_t name_records_count;	//Name Records count
		uint16_t storage_offset;		//Offset for strings storage, from start of the table
	};

	struct tt_name_record
	{
		uint16_t platform_id;
		uint16_t encoding_id;
		uint16_t language_id;
		uint16_t name_id;
		uint16_t string_length;
		uint16_t string_offset; //from start of storage area
	};
public:

	truetype(const char* filename)
	{
		FILE* ifs = fopen(filename, "rb");
		if (!ifs)
			return;

		tt_offset_table offset_table;
		if (fread(ifs, reinterpret_cast<char*>(&offset_table), sizeof offset_table) != sizeof offset_table)
			return;

		const size_t num_of_tables = _m_swap(offset_table.num_of_tables);
		for (size_t i = 0; i < num_of_tables; ++i)
		{
			tt_table_directory table_directory;
			if (fread(ifs, reinterpret_cast<char*>(&table_directory), sizeof table_directory) != sizeof table_directory)
				return;

			if (*reinterpret_cast<const uint32_t*>("name") == reinterpret_cast<uint32_t&>(table_directory.name))
			{
				//const size_t length = _m_swap(table_directory.length);
				const size_t directory_offset = _m_swap(table_directory.offset);
#define SEEK_BEG 0
				fseek(ifs, directory_offset, SEEK_BEG);

				tt_name_table_header name_table;
				if (fread(ifs, reinterpret_cast<char*>(&name_table), sizeof name_table) != sizeof name_table)
					return;

				const size_t name_records_count = _m_swap(name_table.name_records_count);
				const size_t storage_offset = _m_swap(name_table.storage_offset);

				for (size_t u = 0; u < name_records_count; ++u)
				{
					tt_name_record record;
					if (fread(ifs, reinterpret_cast<char*>(&record), sizeof record) != sizeof record)
						return;

					if ((0 == record.string_length) || (0x100 != record.name_id))
						continue;

					size_t string_length = _m_swap(record.string_length);

					int const filepos = ftell(ifs);
					fseek(ifs, directory_offset + _m_swap(record.string_offset) + storage_offset, SEEK_BEG);

          char* text = font_family_;

          ASSERT(string_length<countof(text));
					//Check if it is unicode
					if ((0 == record.platform_id) || (record.platform_id == 0x300 && record.encoding_id == 0x100))
					{
						if (0 == (string_length & 1)) //the string_length must be
						{
							//This is unicode
							MYREALLOC(text, string_length+1);
							fread(ifs, text, string_length);

							for (size_t i = 0; i < string_length; i += 2)
								T_SWAP(char, text[i], text[i + 1]);
							
							// codecvt(text, text, unicode::utf16).to_bytes(unicode::utf8);
						}
					}
					else
					{
						MYREALLOC(text, string_length+1);
						fread(ifs, text, string_length);
					}

					if (*text)
					{
						switch (record.name_id)
						{
						case 0x100:
							break;
						case 0x400:
							text[0] = 0;
						}
					}
					fseek(ifs, filepos, SEEK_BEG);
				}
			}
		}
	}

	const char* font_family() const
	{
		return font_family_;
	}
private:
	static uint16_t _m_swap(uint16_t val)
	{
		return (val << 8) | (val >> 8);
	}

	static uint32_t _m_swap(uint32_t val)
	{
		return (static_cast<uint32_t>(_m_swap(uint16_t(val & 0xFFFF))) << 16) | _m_swap(uint16_t(val >> 16));
	}
private:
	char font_family_[32];
};

#endif