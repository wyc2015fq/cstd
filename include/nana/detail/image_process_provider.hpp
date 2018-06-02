#ifndef NANA_PAINT_IMAGE_PROCESS_PROVIDER_HPP
#define NANA_PAINT_IMAGE_PROCESS_PROVIDER_HPP
#include <nana/pat/cloneable.hpp>
#include <nana/paint/image_process_interface.hpp>
#include <string>
#include <map>

namespace nana
{
	namespace paint
	{
		namespace detail
		{
			class image_process_provider
				: noncopyable
			{
				image_process_provider();

				struct stretch_tag
				{
					typedef image_process::stretch_interface	interface_t;
					typedef pat::mutable_cloneable<interface_t>	cloneable_t;
					typedef	map<string, cloneable_t>		table_t;

					table_t table;
					interface_t* employee;
				}stretch_;

				struct alpha_blend_tag
				{
					typedef image_process::alpha_blend_interface	interface_t;
					typedef pat::mutable_cloneable<interface_t>	cloneable_t;
					typedef map<string, cloneable_t>		table_t;

					table_t	table;
					interface_t	* employee;				
				}alpha_blend_;

				struct blend_tag
				{
					typedef image_process::blend_interface	interface_t;
					typedef pat::mutable_cloneable<interface_t>	cloneable_t;
					typedef map<string, cloneable_t>		table_t;

					table_t	table;
					interface_t	* employee;
				}blend_;

				struct line_tag
				{
					typedef image_process::line_interface	interface_t;
					typedef pat::mutable_cloneable<interface_t>	cloneable_t;
					typedef map<string, cloneable_t>		table_t;

					table_t	table;
					interface_t * employee;
				}line_;

				struct blur_tag
				{
					typedef image_process::blur_interface	interface_t;
					typedef pat::mutable_cloneable<interface_t>	cloneable_t;
					typedef map<string, cloneable_t>		table_t;

					table_t	table;
					interface_t * employee;				
				}blur_;
			public:

				static image_process_provider & instance();

				stretch_tag & ref_stretch_tag();
				image_process::stretch_interface * const * stretch() const;
				image_process::stretch_interface * ref_stretch(const char* name) const;

				alpha_blend_tag & ref_alpha_blend_tag();
				image_process::alpha_blend_interface * const * alpha_blend() const;
				image_process::alpha_blend_interface * ref_alpha_blend(const char* name) const;

				blend_tag & ref_blend_tag();
				image_process::blend_interface * const * blend() const;
				image_process::blend_interface * ref_blend(const char* name) const;

				line_tag & ref_line_tag();
				image_process::line_interface * const * line() const;
				image_process::line_interface * ref_line(const char* name) const;

				blur_tag & ref_blur_tag();
				image_process::blur_interface * const * blur() const;
				image_process::blur_interface * ref_blur(const char* name) const;
			public:
				template<typename Tag>
				void set(Tag & tag, const char* name)
				{
					auto i = tag.table.find(name);
					if(i != tag.table.end())
						tag.employee = &(*(i->second));
				}

				//add
				//@brief:	The add operation is successful if the name does not exist.
				//			it does not replace the existing object by new object, becuase this
				//			feature is thread safe and efficiency.
				template<typename ImageProcessor, typename Tag>
				void add(Tag & tag, const char* name)
				{
					if(tag.table.count(name) == 0)
					{
						typedef typename Tag::cloneable_t cloneable_t;
						auto & obj = tag.table[name];
						obj = cloneable_t(ImageProcessor());
						if(NULL == tag.employee)
							tag.employee = &(*obj);
					}
				}
			private:
				template<typename Tag>
				typename Tag::interface_t* _m_read(const Tag& tag, const char* name) const
				{
					auto i = tag.table.find(name);
					return (i != tag.table.end() ? &(*i->second) : tag.employee);
				}
			};
		}
	}
}//end namespace nana
#endif
