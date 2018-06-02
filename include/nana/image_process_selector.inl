
#include <nana/paint/image_process_selector.hpp>

namespace nana
{
	namespace paint
	{
		namespace image_process
		{
			//class selector
			void selector::stretch(const char* name)
			{
				image_process_provider & p = image_process_provider::instance();
				p.set(p.ref_stretch_tag(), name);
			}

			void selector::alpha_blend(const char* name)
			{
				image_process_provider & p = image_process_provider::instance();
				p.set(p.ref_alpha_blend_tag(), name);
			}

			void selector::blend(const char* name)
			{
				image_process_provider & p = image_process_provider::instance();
				p.set(p.ref_blend_tag(), name);
			}

			void selector::line(const char* name)
			{
				image_process_provider & p = image_process_provider::instance();
				p.set(p.ref_line_tag(), name);			
			}

			void selector::blur(const char* name)
			{
				image_process_provider & p = image_process_provider::instance();
				p.set(p.ref_blur_tag(), name);			
			}
			//end class selector
		}
	}
}
