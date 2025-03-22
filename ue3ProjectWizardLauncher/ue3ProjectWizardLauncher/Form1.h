#pragma once

#include <fstream>  // For file handling
#include <string>   // For string operations

namespace ue3ProjectWizardLauncher {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Windows::Forms;
	using namespace System::Diagnostics; // For running the process
	using namespace System::IO;          // For file handling

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			savedExePath = L"mvsExePath.txt"; // Initialize here, inside the constructor
			splashTimerCounter = 0; // Initialize the counter here
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:

		System::Windows::Forms::PictureBox^ pictureBox1;
		System::Windows::Forms::PictureBox^ splashPictureBox; // Splash screen PictureBox

		// Path to store the Unreal Engine executable path
		String^ savedExePath; // Don't initialize here

		// Splash timer
		System::Windows::Forms::Timer^ splashTimer;

		// Visual Studio process
		Process^ visualStudioProcess;

		// Counter to track how long the splash screen has been open
		int splashTimerCounter; // Declare here, initialize in the constructor

		// Event handler for the splash timer tick
		System::Void OnSplashTimerTick(System::Object^ sender, System::EventArgs^ e)
		{
			// Increment the counter
			splashTimerCounter++;

			// Check if the Visual Studio process is running
			if (visualStudioProcess != nullptr && !visualStudioProcess->HasExited)
			{
				// Refresh the process information to get the latest window title
				visualStudioProcess->Refresh();
			} else {
				// Stop the splash timer
				splashTimer->Stop();

				// Wait for 2 seconds (2000 milliseconds)
				System::Threading::Thread::Sleep(2000);

				// Show a message box with instructions for the user
				MessageBox::Show(L"Select from the list: Unreal Engine 3 Project Wizard.", 
					L"Create new Unreal Engine 3 Project", 
					MessageBoxButtons::OK, 
					MessageBoxIcon::Information);
				//
				this->Close();
			}

			// Fallback: Close the splash screen after 20 seconds (to avoid hanging indefinitely)
			if (splashTimerCounter >= 20)
			{
				this->Close();
			}
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->splashPictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->splashTimer = (gcnew System::Windows::Forms::Timer(this->components));
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splashPictureBox))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(0, 0);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(100, 50);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// splashPictureBox
			// 
			this->splashPictureBox->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"splashPictureBox.Image")));
			this->splashPictureBox->Location = System::Drawing::Point(0, 6);
			this->splashPictureBox->Name = L"splashPictureBox";
			this->splashPictureBox->Size = System::Drawing::Size(237, 249);
			this->splashPictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->splashPictureBox->TabIndex = 1;
			this->splashPictureBox->TabStop = false;
			this->splashPictureBox->Click += gcnew System::EventHandler(this, &Form1::splashPictureBox_Click);
			// 
			// splashTimer
			// 
			this->splashTimer->Interval = 1000;
			this->splashTimer->Tick += gcnew System::EventHandler(this, &Form1::OnSplashTimerTick);
			// 
			// Form1
			// 
			this->AccessibleDescription = L"ue3ProjectWizardLauncher";
			this->AccessibleName = L"ue3ProjectWizardLauncher";
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->ClientSize = System::Drawing::Size(237, 261);
			this->Controls->Add(this->splashPictureBox);
			this->Controls->Add(this->pictureBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"ue3ProjectWizardLauncher";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splashPictureBox))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: 
		System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {
			// Center the splash screen PictureBox
			splashPictureBox->Left = (this->ClientSize.Width - splashPictureBox->Width) / 2;
			splashPictureBox->Top = (this->ClientSize.Height - splashPictureBox->Height) / 2;
			splashPictureBox->BringToFront(); // Ensure it's on top

			// Check if the path is already saved in the configuration file
			String^ visualStudioPath = GetSavedExePath();

			if (String::IsNullOrEmpty(visualStudioPath))
			{
				// Path is not saved, show the OpenFileDialog to select the file
				OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
				openFileDialog->Title = L"Select Visual Studio devenv.exe";
				openFileDialog->Filter = L"Executable Files (*.exe)|*.exe|All Files (*.*)|*.*";
				// If no file was selected, show an informational message
				MessageBox::Show(L"Please select C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\Common7\\IDE\\devenv.exe", 
					L"Selection Required", 
					MessageBoxButtons::OK, 
					MessageBoxIcon::Information);

				// Show the dialog to the user and check if a file was selected
				if (openFileDialog->ShowDialog() == Windows::Forms::DialogResult::OK)
				{
					visualStudioPath = openFileDialog->FileName;

					// Save the selected path to the file
					SaveExePath(visualStudioPath);
				}
				else
				{
					// If no file was selected, show an error message
					MessageBox::Show(L"Please select C:\\Program Files (x86)\\Microsoft Visual Studio 9.0\\Common7\\IDE\\devenv.exe", 
						L"Error", 
						MessageBoxButtons::OK, 
						MessageBoxIcon::Error);

					// Optionally, close the form or re-enable file selection
					this->Close();
					return;
				}
			}

			// Hide the splash screen
			splashPictureBox->Visible = true;

			// Wait for 2 seconds (3000 milliseconds)
			System::Threading::Thread::Sleep(3000);


			// The command to start a new project wizard in Visual Studio
			String^ command = L"/Command File.NewProject";

			// Prepare the ProcessStartInfo
			ProcessStartInfo^ startInfo = gcnew ProcessStartInfo();
			startInfo->FileName = visualStudioPath;  // Set the selected executable as the filename
			startInfo->Arguments = command;     // Command to open the New Project wizard
			startInfo->UseShellExecute = false; // Don't use shell
			startInfo->CreateNoWindow = true;   // Hide the CMD window


			// Start the process
			System::Diagnostics::Process::Start(startInfo);

			// Start the splash timer
			splashTimer->Start();
		};

		// Method to save the executable path to a file
		void SaveExePath(String^ path)
		{
			try
			{
				// Use StreamWriter to save the path to a text file
				System::IO::StreamWriter^ writer = gcnew System::IO::StreamWriter(savedExePath);
				writer->WriteLine(path);
				writer->Close();
			}
			catch (Exception^ e)
			{
				MessageBox::Show(L"Error saving the executable path: " + e->Message, 
					L"Error", 
					MessageBoxButtons::OK, 
					MessageBoxIcon::Error);
			}
		}

		// Method to retrieve the saved executable path from the file
		String^ GetSavedExePath()
		{
			try
			{
				if (System::IO::File::Exists(savedExePath))
				{
					// Use StreamReader to read the saved path from the file
					System::IO::StreamReader^ reader = gcnew System::IO::StreamReader(savedExePath);
					String^ path = reader->ReadLine();
					reader->Close();
					return path;
				}
			}
			catch (Exception^ e)
			{
				MessageBox::Show(L"Error reading the saved executable path: " + e->Message, 
					L"Error", 
					MessageBoxButtons::OK, 
					MessageBoxIcon::Error);
			}

			return nullptr;
		}
	private: System::Void splashPictureBox_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	};
}