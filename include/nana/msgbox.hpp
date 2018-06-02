/*
*	A Message Box Class
*	Nana C++ Library(http://www.nanapro.org)
*	Copyright(C) 2003-2017 Jinhao(cnjinhao@hotmail.com)
*
*	Distributed under the Boost Software License, Version 1.0.
*	(See accompanying file LICENSE_1_0.txt or copy at
*	http://www.boost.org/LICENSE_1_0.txt)
*
*	@file: nana/gui/msgbox.hpp
*/

#ifndef NANA_GUI_MSGBOX_HPP
#define NANA_GUI_MSGBOX_HPP


#include <sstream>

namespace nana
{
	//Forward declaration of filebox for msgbox
	class filebox;

	/// Prefabricated modal dialog box (with text, icon and actions buttons) that inform and instruct the user.
	class msgbox
	{
	public:
		/// Identifiers of icons.
		enum icon_t{icon_none, icon_information, icon_warning, icon_error, icon_question};

		/// Identifiers of buttons.
		enum button_t{ok, yes_no, yes_no_cancel};

		/// Identifiers of buttons that a user clicked.
		enum pick_t{pick_ok, pick_yes, pick_no, pick_cancel};

		/// Default construct that creates a message box with default title and default button, the default button is OK.
		msgbox();

		/// Copy construct from an existing msgbox object.
		msgbox(const msgbox&);

		/// Assign from an existing msgbox object.
		msgbox& operator=(const msgbox&);

		/// Construct that creates a message box with a specified title and default button.
		msgbox(const char*);

		/// Construct that creates a message box with an owner windoow, a specified title and buttons.
		msgbox(window, const char*, button_t = ok);

		/// Sets an icon for informing user.
		msgbox& icon(icon_t);

		/// Clears the text message buffer.
		void clear();

		/// Writes a string to the buffer.
		msgbox & operator<<(const wchar*);

		/// Writes a string to the buffer.
		msgbox & operator<<(const wchar_t*);

		/// Writes a UTF-8 string to the buffer.
		msgbox & operator<<(const char*);

		/// Writes a UTF-8 string to the buffer.
		msgbox & operator<<(const char*);

		/// Writes a string to the buffer.
		msgbox & operator<<(const charset&);

		// Calls a manipulator to the stream.
		msgbox & operator<<(ostream& (*)(ostream&));

		/// Write a streamizable object to the buffer.
		template<typename T>
		msgbox & operator<<(const T& t)
		{
			sstream_<<t;
			return *this;
		}

		/// \brief Displays the message that buffered in the stream.
		/// @return, the button that user clicked.
		pick_t show() const;

		/// A function object method alternative to show()
		pick_t operator()() const
		{
			return show();
		}
	private:
		stringstream sstream_;
		window wd_;
		string title_;
		button_t button_;
		icon_t icon_;
	};

	class inputbox
	{
		struct abstract_content
		{
			virtual ~abstract_content() = default;

			virtual const char* label() const = 0;
			virtual window create(window, unsigned label_px) = 0;
			virtual unsigned fixed_pixels() const;
		};

	public:
		class boolean
			: public abstract_content
		{
			struct implement;
		public:
			boolean(::string label, bool initial_value);
			~boolean();

			bool value() const;
		private:
			//Implementation of abstract_content
			const char* label() const;
			window create(window, unsigned label_px);
			unsigned fixed_pixels() const;
		private:
			unique_ptr<implement> impl_;
		};

		class integer
			: public abstract_content
		{
			struct implement;
		public:
			integer(::string label, int init_value, int begin, int last, int step);
			~integer();

			int value() const;
		private:
			//Implementation of abstract_content
			const char* label() const;
			window create(window, unsigned label_px);
			unsigned fixed_pixels() const;
		private:
			unique_ptr<implement> impl_;
		};

		class real
			: public abstract_content
		{
			struct implement;
		public:
			real(::string label, double init_value, double begin, double last, double step);
			~real();

			double value() const;
		private:
			//Implementation of abstract_content
			const char* label() const;
			window create(window, unsigned label_px);
			unsigned fixed_pixels() const;
		private:
			unique_ptr<implement> impl_;
		};

		class text
			: public abstract_content
		{
			struct implement;

			text(const text&) {}
			text& operator=(const text&) {}
		public:
			text(::string label, ::string init_text = ::string());
			text(::string label, vector<::string>);

			~text();

			void tip_string(wstring tip);
			void tip_string(string tip_utf8);

			void mask_character(wchar_t ch);

			::string value() const;
		private:
			//Implementation of abstract_content
			const char* label() const;
			window create(window, unsigned label_px);
			unsigned fixed_pixels() const;
		private:
			unique_ptr<implement> impl_;
		};

		class date
			: public abstract_content
		{
			struct implement;
		public:
			date(::string label);

			~date();

			::string value() const;
			int year() const;
			int month() const;	//[1, 12]
			int day() const;	//[1, 31]
		private:
			//Implementation of abstract_content
			const char* label() const;
			window create(window, unsigned label_px);
			unsigned fixed_pixels() const;
		private:
			unique_ptr<implement> impl_;
		};

		class path
			: public abstract_content
		{
			struct implement;
		public:
			path(::string label, const filebox&);
			~path();

			::string value() const;
		private:
			//Implementation of abstract_content
			const char* label() const;
			window create(window, unsigned label_px);
		private:
			unique_ptr<implement> impl_;
		};

		inputbox(window, ::string description, ::string title = ::string());

		void image(image, bool is_left, const rectangle& valid_area = {});
		void image_v(image, bool is_top, const rectangle& valid_area = {});

		template<typename ...Args>
		bool show(Args&& ... args)
		{
			vector<abstract_content*> contents;
			_m_fetch_args(contents, forward<Args>(args)...);
			if (contents.empty())
				return false;

			return _m_open(contents, false);
		}

		template<typename ...Args>
		bool show_modal(Args&& ... args)
		{
			vector<abstract_content*> contents;
			_m_fetch_args(contents, forward<Args>(args)...);

			if (contents.empty())
				return false;

			return _m_open(contents, true);
		}

		/// Sets a verifier to verify the user input.
		void verify(function<bool(window)> verifier);

		/** Sets the minimum width for the entry fields
            @param[in] pixels  new minimum width

            If not called, the default is 100 pixels
        */
		void min_width_entry_field( unsigned pixels );

	private:
		void _m_fetch_args(vector<abstract_content*>&);

		template<typename ...Args>
		void _m_fetch_args(vector<abstract_content*>& contents, abstract_content& content, Args&&... args)
		{
			contents.push_back(&content);
			_m_fetch_args(contents, forward<Args>(args)...);
		}

		bool _m_open(vector<abstract_content*>&, bool modal);
	private:
		window owner_;
		::string description_;
		::string title_;
		function<bool(window)> verifier_;
		image images_[4];
		rectangle valid_areas_[4];
        unsigned min_width_entry_field_pixels_;
	};
}//end namespace nana


#endif
