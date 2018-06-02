#ifndef NANA_PAINT_DETAIL_IMAGE_IMPL_INTERFACE_HPP
#define NANA_PAINT_DETAIL_IMAGE_IMPL_INTERFACE_HPP

#include "../image.hpp"
#include <nana/filesystem/filesystem_ext.hpp>

namespace nana{	namespace paint{

	// class image::image_impl_interface
	//		the image refers to an object of image::image_impl_interface by refer. Employing refer to refer the image::implement_t object indirectly is used
	//	for saving the memory that sharing the same image resource with many image objects.
	class image::image_impl_interface
		: private noncopyable
	{
		image_impl_interface& operator=(const image_impl_interface& rhs);
	public:
		using graph_reference = graphics&;
		virtual ~image_impl_interface() = 0;	//The destructor is defined in ../image.cpp
		virtual bool open(const path& file) = 0;
		virtual bool open(const void* data, size_t bytes) = 0; // reads image from memory
		virtual bool alpha_channel() const = 0;
		virtual bool empty() const = 0;
		virtual void close() = 0;
		virtual size size() const = 0;
		virtual void paste(const rectangle& src_r, graph_reference dst, const point& p_dst) const = 0;
		virtual void stretch(const rectangle& src_r, graph_reference dst, const rectangle& r) const = 0;
	};//end class image::image_impl_interface
}//end namespace paint
}//end namespace nana

#endif
