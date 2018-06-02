
#ifndef NANA_GUI_EFFECTS_HPP
#define NANA_GUI_EFFECTS_HPP


		enum edge_nimbus
		{
			none, active = 0x1, over = 0x2
		};

		class bground_interface
		{
		public:
			typedef graphics & graph_reference;

			virtual ~bground_interface() = 0;
			virtual void take_effect(window, graph_reference) const = 0;
		};

		class bground_factory_interface
		{
			friend class effects_accessor;
		public:
			virtual ~bground_factory_interface() = 0;
		private:
			virtual bground_interface * create() const = 0;
		};

		class bground_transparent
			: public bground_factory_interface
		{
		public:
			bground_transparent(size_t percent);
		private:
			bground_interface* create() const;
		private:
			size_t percent_;
		};

		class bground_blur
			: public bground_factory_interface
		{
		public:
			bground_blur(size_t radius);
		private:
			bground_interface * create() const;
		private:
			size_t radius_;
		};
	}

#endif
