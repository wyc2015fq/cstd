#pragma once

#include <string.h>
#include "Warper.h"

	using namespace ImageWarper;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
#if 0
	class Form1
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			
			m_bmpImage = nullptr;
			m_warper = nullptr;
			m_iRadius = 40;
			cmbWarperType->SelectedIndex = 0;
			m_iWarperType = WARPER_TRANSLATE;
			m_penRadius = CreatePen(System::Drawing::Color::White);
			m_fontWarper = gcnew System::Drawing::Font("Tahoma", 10);
			m_brWarper = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Black);
			m_ptImgTopLeft.X = lblWarperType->Location.X;
			m_ptImgTopLeft.Y = cmbWarperType->Location.Y + cmbWarperType->Size.Height + 3;
			m_bCreatingWarpDone = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			delete m_warper;
		}
	private: Timer^  m_timerGrowShrink;
	private: Button^  btnBrowse;

	private: TextBox^  txtFilePath;
	private: Label^  lblWarperType;

	private: ComboBox^  cmbWarperType;
	private: Label^  lblBrushSize;




	protected: 
	private: IContainer^  components;

		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->m_timerGrowShrink = (gcnew System::Windows::Forms::Timer(this->components));
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->txtFilePath = (gcnew System::Windows::Forms::TextBox());
			this->lblWarperType = (gcnew System::Windows::Forms::Label());
			this->cmbWarperType = (gcnew System::Windows::Forms::ComboBox());
			this->lblBrushSize = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// m_timerGrowShrink
			// 
			this->m_timerGrowShrink->Tick += gcnew System::EventHandler(this, &Form1::m_timerGrowShrink_Tick);
			// 
			// btnBrowse
			// 
			this->btnBrowse->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->btnBrowse->Location = System::Drawing::Point(563, 3);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(40, 23);
			this->btnBrowse->TabIndex = 0;
			this->btnBrowse->Text = L"...";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &Form1::btnBrowse_Click);
			// 
			// txtFilePath
			// 
			this->txtFilePath->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtFilePath->Location = System::Drawing::Point(3, 3);
			this->txtFilePath->Name = L"txtFilePath";
			this->txtFilePath->ReadOnly = true;
			this->txtFilePath->Size = System::Drawing::Size(557, 20);
			this->txtFilePath->TabIndex = 1;
			// 
			// lblWarperType
			// 
			this->lblWarperType->AutoSize = true;
			this->lblWarperType->Location = System::Drawing::Point(3, 33);
			this->lblWarperType->Name = L"lblWarperType";
			this->lblWarperType->Size = System::Drawing::Size(45, 13);
			this->lblWarperType->TabIndex = 2;
			this->lblWarperType->Text = L"Warper:";
			// 
			// cmbWarperType
			// 
			this->cmbWarperType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmbWarperType->FormattingEnabled = true;
			this->cmbWarperType->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"Translate", L"Grow", L"Shrink"});
			this->cmbWarperType->Location = System::Drawing::Point(50, 30);
			this->cmbWarperType->Name = L"cmbWarperType";
			this->cmbWarperType->Size = System::Drawing::Size(121, 21);
			this->cmbWarperType->TabIndex = 3;
			this->cmbWarperType->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::cmbWarperType_SelectedIndexChanged);
			// 
			// lblBrushSize
			// 
			this->lblBrushSize->AutoSize = true;
			this->lblBrushSize->Location = System::Drawing::Point(177, 34);
			this->lblBrushSize->Name = L"lblBrushSize";
			this->lblBrushSize->Size = System::Drawing::Size(212, 13);
			this->lblBrushSize->TabIndex = 4;
			this->lblBrushSize->Text = L"Use [ and ] keys to change the brush size :)";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(609, 368);
			this->Controls->Add(this->lblBrushSize);
			this->Controls->Add(this->cmbWarperType);
			this->Controls->Add(this->lblWarperType);
			this->Controls->Add(this->txtFilePath);
			this->Controls->Add(this->btnBrowse);
			this->DoubleBuffered = true;
			this->KeyPreview = true;
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseUp);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::Form1_Paint);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseDown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::Form1_KeyPress);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::Form1_MouseMove);
			this->ResumeLayout(false);
			this->PerformLayout();

		}

	private:
		Warper* m_warper;
		int m_iRadius;
    bool m_bMouseDown;
		int m_iWarperType;
    Point m_ptMouse;
		HBITMAP m_bmpImage;
		Pen m_penRadius;
		Font m_fontWarper;
		Brush m_brWarper;
		Point m_ptImgTopLeft;
		bool m_bCreatingWarpDone;

		System::Void Form1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
		{
			if (!m_warper)
				return;

			System::Drawing::Rectangle srcRect(0, 0, m_bmpImage->Width, m_bmpImage->Height);
			System::Drawing::Rectangle  dstRect(m_ptImgTopLeft.X, m_ptImgTopLeft.Y,
				m_bmpImage->Width, m_bmpImage->Height);
			e->Graphics->DrawImage(m_bmpImage, dstRect, srcRect, System::Drawing::GraphicsUnit::Pixel);
			e->Graphics->DrawArc(m_penRadius, 
				System::Drawing::Rectangle(m_ptMouse.X - m_iRadius, m_ptMouse.Y - m_iRadius, 2*m_iRadius, 2*m_iRadius),
				0, 360);
			//e->Graphics->DrawString(System::String::Format("Warper Type: {0}", m_iWarperType),
			//	m_fontWarper, m_brWarper, 
			//	System::Drawing::RectangleF(0.0f, m_bmpImage->Height, m_bmpImage->Width, 30.0f));
		}

		System::Void Form1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if (e->Button == System::Windows::Forms::MouseButtons::Left && m_bmpImage && m_bCreatingWarpDone)
			{
				Point pt;
				pt.X = e->Location.X - m_ptImgTopLeft.X;
				pt.Y = e->Location.Y - m_ptImgTopLeft.Y;
				if (pt.X >= 0 && pt.X < m_bmpImage->Width &&
					pt.Y >= 0 && pt.Y < m_bmpImage->Height)
				{
					m_warper->BeginWarp(pt, m_iRadius, m_iWarperType);
					m_bMouseDown = true;
					if (m_iWarperType != WARPER_TRANSLATE)
					{
						m_timerGrowShrink->Enabled = true;
					}
				}
			}
		}
		System::Void Form1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			m_ptMouse = e->Location;
			if (e->Button == System::Windows::Forms::MouseButtons::Left && m_bmpImage && m_bCreatingWarpDone)
			{
				Point pt;
				pt.X = e->Location.X - m_ptImgTopLeft.X;
				pt.Y = e->Location.Y - m_ptImgTopLeft.Y;
				if (
					m_iWarperType == WARPER_TRANSLATE &&
					pt.X >= 0 && pt.X < m_bmpImage->Width &&
					pt.Y >= 0 && pt.Y < m_bmpImage->Height)
				{
					WarpedImage* warpedImg = m_warper->UpdateWarp(pt);
					if(warpedImg)
						DrawImage(m_bmpImage, warpedImg, false);
				}
			}
			Invalidate();
		}

		System::Void DrawImage(System::Drawing::Bitmap^ bmpDest, WarpedImage* imgData, bool bEnd)
		{
			System::Drawing::Graphics^ grp = System::Drawing::Graphics::FromImage(bmpDest);
			System::Drawing::Bitmap^ bmp = gcnew 
				System::Drawing::Bitmap(imgData->Image.Width, imgData->Image.Height, grp);
			System::Drawing::Imaging::BitmapData^ bmpData = bmp->LockBits(
				System::Drawing::Rectangle(0, 0, imgData->Image.Width, imgData->Image.Height),
				System::Drawing::Imaging::ImageLockMode::ReadWrite,
				System::Drawing::Imaging::PixelFormat::Format24bppRgb);
			System::Diagnostics::Debug::Assert(bmpData->Stride == imgData->Image.ScanWidth);
			memcpy((char*)(void*)bmpData->Scan0, imgData->Image.Data, bmpData->Stride*bmpData->Height);
			bmp->UnlockBits(bmpData);

			grp->DrawImage(bmp, System::Drawing::Point(imgData->Position.X, imgData->Position.Y));
			/*if(m_warper && chkShowGrid->Checked && !bEnd)
			{
				PointArray2D* arrPts = m_warper->m_canvas->m_GridPoints;
				System::Drawing::Pen^ pt = gcnew System::Drawing::Pen(System::Drawing::Color::Red);
				for(int i = 0; i < arrPts->GetWidth() - 1; ++i)
				{
					for(int j = 0; j < arrPts->GetHeight() - 1; ++j)
					{
						grp->DrawLine(pt, arrPts->GetItem(i, j)->X,
							arrPts->GetItem(i, j)->Y,
							arrPts->GetItem(i, j+1)->X,
							arrPts->GetItem(i, j+1)->Y);
						grp->DrawLine(pt, 
							arrPts->GetItem(i, j)->X,
							arrPts->GetItem(i, j)->Y, 
							arrPts->GetItem(i+1, j)->X,
							arrPts->GetItem(i+1, j)->Y);
					}
				}
			}*/
		}

		System::Void LoadImage()
		{
			System::String^ sFile = gcnew System::String(txtFilePath->Text);
			try
			{
				m_bmpImage = gcnew System::Drawing::Bitmap(sFile);
			}
			catch(Exception^ /*e*/)
			{
				MessageBox::Show(this, L"Error when loading image.", L"Error");
				return;
			}
			
			BitmapData^ bmpData = m_bmpImage->LockBits(
				System::Drawing::Rectangle(0, 0, m_bmpImage->Width, m_bmpImage->Height),
				ImageLockMode::ReadWrite, PixelFormat::Format24bppRgb);
			ImageData imgData;
			imgData.Resize(bmpData->Width, bmpData->Height, 3, bmpData->Stride);
			memcpy(imgData.Data, (char*)(void*)bmpData->Scan0, bmpData->Stride*bmpData->Height);

			m_bmpImage->UnlockBits(bmpData);

			m_warper = new Warper(imgData);
			Invalidate();
		}

		System::Void Form1_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		{
			if (!m_warper || !m_bCreatingWarpDone)
				return;
			if (e->Button == System::Windows::Forms::MouseButtons::Left && m_bmpImage)
			{
				m_bMouseDown = false;

				Point pt;
				pt.X = e->Location.X - m_ptImgTopLeft.X;
				pt.Y = e->Location.Y - m_ptImgTopLeft.Y;
				if (
					pt.X >= 0 && pt.X < m_bmpImage->Width &&
					pt.Y >= 0 && pt.Y < m_bmpImage->Height)
				{
					WarpedImage* warpedImg = m_warper->UpdateWarp(pt);
					if(warpedImg)
					{
						warpedImg = m_warper->EndWarp(warpedImg);

						if (m_iWarperType != WARPER_TRANSLATE)
						{
							m_timerGrowShrink->Enabled = false;
						}

						DrawImage(m_bmpImage, warpedImg, false);
					}
				}
				Invalidate();
			}
		}
	
		System::Void Form1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
		{
			bool bInvalidate = false;

			switch(e->KeyChar)
			{
			case '[':
				{
					if (!m_bMouseDown && m_iRadius > 10)
					{
						m_iRadius -= 5;
						bInvalidate = true;
					}
				}
				break;
			case ']':
				{
					if (!m_bMouseDown && m_iRadius < 45)
					{
						m_iRadius += 5;
						bInvalidate = true;
					}
				}
				break;
			}

			if (bInvalidate)
				Invalidate();
		}

		
		System::Void m_timerGrowShrink_Tick(System::Object^  sender, System::EventArgs^  e)
		{
			if (!m_warper || !m_bCreatingWarpDone)
				return;

			if (m_iWarperType != WARPER_TRANSLATE && m_bMouseDown)
			{
				Point pt;
				pt.X = m_ptMouse.X - m_ptImgTopLeft.X;
				pt.Y = m_ptMouse.Y - m_ptImgTopLeft.Y;
				if (m_bmpImage &&
					pt.X >= 0 && pt.X < m_bmpImage->Width &&
					pt.Y >= 0 && pt.Y < m_bmpImage->Height)
				{
					WarpedImage* warpedImg = m_warper->UpdateWarp(pt);
					DrawImage(m_bmpImage, warpedImg, false);
					Invalidate();
				}
			}
			else
			{
				m_timerGrowShrink->Enabled = false;
			}
		}

		System::Void cmbWarperType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		{
			if (!this->Created || m_bMouseDown)
				return;

			switch (cmbWarperType->SelectedIndex)
			{
				case 0:
				{
 					if (m_iWarperType != WARPER_TRANSLATE)
 					{
 						m_iWarperType = WARPER_TRANSLATE;
 					}
 				}
 				break;
 			case 1:
 				{
 					if (m_iWarperType != WARPER_GROW)
 					{
 						m_iWarperType = WARPER_GROW;
 					}
 				}
 				break;
 			case 2:
 				{
 					if (m_iWarperType != WARPER_SHRINK)
 					{
 						m_iWarperType = WARPER_SHRINK;
 					}
 				}
 				break;
			}
		}

		System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e)
		{
			OpenFileDialog dlg;
			m_bCreatingWarpDone = false;
			dlg.Filter = "Images (*.*)|*.*||";
			if (dlg.ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				System::String^ filename = System::IO::Path::GetFileName(dlg.FileName); 
				System::String^  path = System::IO::Path::GetDirectoryName(dlg.FileName);
				txtFilePath->Text = System::IO::Path::Combine(path, filename);
				LoadImage();
			}
			m_bCreatingWarpDone = true;
			
		}
};
#endif
