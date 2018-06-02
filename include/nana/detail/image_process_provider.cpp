
#include <nana/paint/detail/image_process_provider.hpp>

#include <nana/paint/detail/image_processor.hpp>

namespace nana
{
namespace paint
{
	namespace detail
	{
	//class image_process_provider
		image_process_provider& image_process_provider::instance()
		{
			static image_process_provider obj;
			return obj;
		}

		image_process_provider::image_process_provider()
		{
			add<algorithms::bilinear_interoplation>(stretch_, "bilinear interoplation");
			add<algorithms::proximal_interoplation>(stretch_, "proximal interoplation");
			add<algorithms::alpha_blend>(alpha_blend_, "alpha_blend");
			add<algorithms::blend>(blend_, "blend");
			add<algorithms::bresenham_line>(line_, "bresenham_line");
			add<algorithms::superfast_blur>(blur_, "superfast_blur");
		}

		image_process_provider::stretch_tag& image_process_provider::ref_stretch_tag()
		{
			return stretch_;
		}

		image_process::stretch_interface * const * image_process_provider::stretch() const
		{
			return &stretch_.employee;
		}

		image_process::stretch_interface * image_process_provider::ref_stretch(const char* name) const
		{
			return _m_read(stretch_, name);
		}

		//alpha_blend
		image_process_provider::alpha_blend_tag& image_process_provider::ref_alpha_blend_tag()
		{
			return alpha_blend_;
		}

		image_process::alpha_blend_interface * const * image_process_provider::alpha_blend() const
		{
			return &alpha_blend_.employee;
		}

		image_process::alpha_blend_interface * image_process_provider::ref_alpha_blend(const char* name) const
		{
			return _m_read(alpha_blend_, name);
		}
		
		//blend
		image_process_provider::blend_tag& image_process_provider::ref_blend_tag()
		{
			return blend_;
		}

		image_process::blend_interface * const * image_process_provider::blend() const
		{
			return &blend_.employee;
		}

		image_process::blend_interface * image_process_provider::ref_blend(const char* name) const
		{
			return _m_read(blend_, name);
		}

		//line
		image_process_provider::line_tag & image_process_provider::ref_line_tag()
		{
			return line_;
		}

		image_process::line_interface * const * image_process_provider::line() const
		{
			return &line_.employee;
		}

		image_process::line_interface * image_process_provider::ref_line(const char* name) const
		{
			return _m_read(line_, name);
		}

		//Blur
		image_process_provider::blur_tag & image_process_provider::ref_blur_tag()
		{
			return blur_;
		}

		image_process::blur_interface * const * image_process_provider::blur() const
		{
			return &blur_.employee;
		}

		image_process::blur_interface * image_process_provider::ref_blur(const char* name) const
		{
			return _m_read(blur_, name);
		}
	//end class image_process_provider
	}
}
}//end namespace nana
