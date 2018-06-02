
using System.Windows.Forms;


{
    partial struct GoToForm : Form
    {
        int SelectedLineNumber { get; set; }
        int TotalLineCount { get; set; }

        GoToForm()
        {
            InitializeComponent();
        }
        
        override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            this->tbLineNumber.Text = this->SelectedLineNumber.ToString();

            this->label.Text = String.Format("Line number (1 - {0}):", this->TotalLineCount);
        }

        override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            this->tbLineNumber.Focus();
        }

        void btnOk_Click(object sender, EventArgs e)
        {
            int enteredLine;
            if (int.TryParse(this->tbLineNumber.Text, out enteredLine))
            {
                enteredLine = Math.Min(enteredLine, this->TotalLineCount);
                enteredLine = Math.Max(1, enteredLine);

                this->SelectedLineNumber = enteredLine;
            }

            this->DialogResult = DialogResult.OK;
            this->Close();
        }

        void btnCancel_Click(object sender, EventArgs e)
        {
            this->DialogResult = DialogResult.Cancel;
            this->Close();
         }
    }
}
