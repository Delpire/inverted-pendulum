namespace EvaUI
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.mainTableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.outputDirectoryTableLayoutPanel = new System.Windows.Forms.TableLayoutPanel();
            this.outputDirectoryLabel = new System.Windows.Forms.Label();
            this.outputDirectoryBrowseButton = new System.Windows.Forms.Button();
            this.outputDirectoryTextBox = new System.Windows.Forms.TextBox();
            this.displaySplitContainer = new System.Windows.Forms.SplitContainer();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.connectButton = new System.Windows.Forms.Button();
            this.refreshPortsButton = new System.Windows.Forms.Button();
            this.portBox = new System.Windows.Forms.ComboBox();
            this.linkStatsGroupBox = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.transferedPacketsLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.transferedPacketsTextBox = new System.Windows.Forms.TextBox();
            this.receivedPacketsTextBox = new System.Windows.Forms.TextBox();
            this.transferBytesLabel = new System.Windows.Forms.Label();
            this.receivedBytesLabel = new System.Windows.Forms.Label();
            this.transferredBytesTextBox = new System.Windows.Forms.TextBox();
            this.receivedBytesTextBox = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.messagesTextBox = new System.Windows.Forms.RichTextBox();
            this.messageLabel = new System.Windows.Forms.Label();
            this.tableLayoutPanel9 = new System.Windows.Forms.TableLayoutPanel();
            this.clearMessagesButton = new System.Windows.Forms.Button();
            this.openSessionDirectoryButton = new System.Windows.Forms.Button();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.drivingModeEnableButton = new System.Windows.Forms.Button();
            this.dataCaptureGroupBox = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.dataFileNameTextBox = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.startCaptureButton = new System.Windows.Forms.Button();
            this.captureTypeList = new System.Windows.Forms.ComboBox();
            this.tableLayoutPanel8 = new System.Windows.Forms.TableLayoutPanel();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.captureRateTextBox = new System.Windows.Forms.NumericUpDown();
            this.captureDurationTextBox = new System.Windows.Forms.NumericUpDown();
            this.dataFileNameLabel = new System.Windows.Forms.Label();
            this.generateFileNameCheckBox = new System.Windows.Forms.CheckBox();
            this.tableLayoutPanel10 = new System.Windows.Forms.TableLayoutPanel();
            this.tiltAngleTextBox = new System.Windows.Forms.TextBox();
            this.tiltAngleLabel = new System.Windows.Forms.Label();
            this.tiltPanel = new System.Windows.Forms.Panel();
            this.mainTableLayoutPanel.SuspendLayout();
            this.outputDirectoryTableLayoutPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.displaySplitContainer)).BeginInit();
            this.displaySplitContainer.Panel1.SuspendLayout();
            this.displaySplitContainer.Panel2.SuspendLayout();
            this.displaySplitContainer.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.linkStatsGroupBox.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel9.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.dataCaptureGroupBox.SuspendLayout();
            this.tableLayoutPanel6.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            this.tableLayoutPanel8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.captureRateTextBox)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.captureDurationTextBox)).BeginInit();
            this.tableLayoutPanel10.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainTableLayoutPanel
            // 
            this.mainTableLayoutPanel.ColumnCount = 1;
            this.mainTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.mainTableLayoutPanel.Controls.Add(this.outputDirectoryTableLayoutPanel, 0, 1);
            this.mainTableLayoutPanel.Controls.Add(this.displaySplitContainer, 0, 0);
            this.mainTableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mainTableLayoutPanel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.mainTableLayoutPanel.Location = new System.Drawing.Point(0, 0);
            this.mainTableLayoutPanel.Name = "mainTableLayoutPanel";
            this.mainTableLayoutPanel.RowCount = 2;
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.mainTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.mainTableLayoutPanel.Size = new System.Drawing.Size(660, 407);
            this.mainTableLayoutPanel.TabIndex = 0;
            // 
            // outputDirectoryTableLayoutPanel
            // 
            this.outputDirectoryTableLayoutPanel.ColumnCount = 3;
            this.outputDirectoryTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 112F));
            this.outputDirectoryTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.outputDirectoryTableLayoutPanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.outputDirectoryTableLayoutPanel.Controls.Add(this.outputDirectoryLabel, 0, 0);
            this.outputDirectoryTableLayoutPanel.Controls.Add(this.outputDirectoryBrowseButton, 2, 0);
            this.outputDirectoryTableLayoutPanel.Controls.Add(this.outputDirectoryTextBox, 1, 0);
            this.outputDirectoryTableLayoutPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.outputDirectoryTableLayoutPanel.Location = new System.Drawing.Point(3, 370);
            this.outputDirectoryTableLayoutPanel.Name = "outputDirectoryTableLayoutPanel";
            this.outputDirectoryTableLayoutPanel.RowCount = 1;
            this.outputDirectoryTableLayoutPanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.outputDirectoryTableLayoutPanel.Size = new System.Drawing.Size(654, 34);
            this.outputDirectoryTableLayoutPanel.TabIndex = 0;
            // 
            // outputDirectoryLabel
            // 
            this.outputDirectoryLabel.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.outputDirectoryLabel.AutoSize = true;
            this.outputDirectoryLabel.Location = new System.Drawing.Point(3, 9);
            this.outputDirectoryLabel.Name = "outputDirectoryLabel";
            this.outputDirectoryLabel.Size = new System.Drawing.Size(106, 16);
            this.outputDirectoryLabel.TabIndex = 0;
            this.outputDirectoryLabel.Text = "Output Directory:";
            // 
            // outputDirectoryBrowseButton
            // 
            this.outputDirectoryBrowseButton.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.outputDirectoryBrowseButton.Location = new System.Drawing.Point(577, 5);
            this.outputDirectoryBrowseButton.Name = "outputDirectoryBrowseButton";
            this.outputDirectoryBrowseButton.Size = new System.Drawing.Size(74, 23);
            this.outputDirectoryBrowseButton.TabIndex = 1;
            this.outputDirectoryBrowseButton.Text = "Browse";
            this.outputDirectoryBrowseButton.UseVisualStyleBackColor = true;
            this.outputDirectoryBrowseButton.Click += new System.EventHandler(this.outputDirectoryBrowseButton_Click);
            // 
            // outputDirectoryTextBox
            // 
            this.outputDirectoryTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.outputDirectoryTextBox.Location = new System.Drawing.Point(115, 6);
            this.outputDirectoryTextBox.Name = "outputDirectoryTextBox";
            this.outputDirectoryTextBox.ReadOnly = true;
            this.outputDirectoryTextBox.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
            this.outputDirectoryTextBox.Size = new System.Drawing.Size(456, 22);
            this.outputDirectoryTextBox.TabIndex = 2;
            // 
            // displaySplitContainer
            // 
            this.displaySplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.displaySplitContainer.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.displaySplitContainer.Location = new System.Drawing.Point(3, 3);
            this.displaySplitContainer.Name = "displaySplitContainer";
            // 
            // displaySplitContainer.Panel1
            // 
            this.displaySplitContainer.Panel1.Controls.Add(this.tableLayoutPanel1);
            // 
            // displaySplitContainer.Panel2
            // 
            this.displaySplitContainer.Panel2.Controls.Add(this.tableLayoutPanel5);
            this.displaySplitContainer.Size = new System.Drawing.Size(654, 361);
            this.displaySplitContainer.SplitterDistance = 351;
            this.displaySplitContainer.TabIndex = 1;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.linkStatsGroupBox, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel3, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 40F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 80F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(351, 361);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 72F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 95F));
            this.tableLayoutPanel2.Controls.Add(this.connectButton, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.refreshPortsButton, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.portBox, 0, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(345, 34);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // connectButton
            // 
            this.connectButton.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.connectButton.Location = new System.Drawing.Point(253, 5);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(89, 23);
            this.connectButton.TabIndex = 0;
            this.connectButton.Text = "Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // refreshPortsButton
            // 
            this.refreshPortsButton.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.refreshPortsButton.Location = new System.Drawing.Point(181, 5);
            this.refreshPortsButton.Name = "refreshPortsButton";
            this.refreshPortsButton.Size = new System.Drawing.Size(66, 23);
            this.refreshPortsButton.TabIndex = 0;
            this.refreshPortsButton.Text = "Refresh";
            this.refreshPortsButton.UseVisualStyleBackColor = true;
            this.refreshPortsButton.Click += new System.EventHandler(this.refreshPortsButton_Click);
            // 
            // portBox
            // 
            this.portBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.portBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.portBox.FormattingEnabled = true;
            this.portBox.Location = new System.Drawing.Point(3, 3);
            this.portBox.Name = "portBox";
            this.portBox.Size = new System.Drawing.Size(172, 24);
            this.portBox.TabIndex = 1;
            // 
            // linkStatsGroupBox
            // 
            this.linkStatsGroupBox.Controls.Add(this.tableLayoutPanel4);
            this.linkStatsGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.linkStatsGroupBox.Location = new System.Drawing.Point(3, 284);
            this.linkStatsGroupBox.Name = "linkStatsGroupBox";
            this.linkStatsGroupBox.Size = new System.Drawing.Size(345, 74);
            this.linkStatsGroupBox.TabIndex = 0;
            this.linkStatsGroupBox.TabStop = false;
            this.linkStatsGroupBox.Text = "Link Stats";
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 4;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Controls.Add(this.transferedPacketsLabel, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.label1, 0, 1);
            this.tableLayoutPanel4.Controls.Add(this.transferedPacketsTextBox, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.receivedPacketsTextBox, 1, 1);
            this.tableLayoutPanel4.Controls.Add(this.transferBytesLabel, 2, 0);
            this.tableLayoutPanel4.Controls.Add(this.receivedBytesLabel, 2, 1);
            this.tableLayoutPanel4.Controls.Add(this.transferredBytesTextBox, 3, 0);
            this.tableLayoutPanel4.Controls.Add(this.receivedBytesTextBox, 3, 1);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 18);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 2;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(339, 53);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // transferedPacketsLabel
            // 
            this.transferedPacketsLabel.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.transferedPacketsLabel.AutoSize = true;
            this.transferedPacketsLabel.Location = new System.Drawing.Point(9, 5);
            this.transferedPacketsLabel.Name = "transferedPacketsLabel";
            this.transferedPacketsLabel.Size = new System.Drawing.Size(78, 16);
            this.transferedPacketsLabel.TabIndex = 0;
            this.transferedPacketsLabel.Text = "Tx Packets:";
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(8, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(79, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "Rx Packets:";
            // 
            // transferedPacketsTextBox
            // 
            this.transferedPacketsTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.transferedPacketsTextBox.Location = new System.Drawing.Point(93, 3);
            this.transferedPacketsTextBox.Name = "transferedPacketsTextBox";
            this.transferedPacketsTextBox.ReadOnly = true;
            this.transferedPacketsTextBox.Size = new System.Drawing.Size(73, 22);
            this.transferedPacketsTextBox.TabIndex = 1;
            this.transferedPacketsTextBox.Text = "0";
            // 
            // receivedPacketsTextBox
            // 
            this.receivedPacketsTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.receivedPacketsTextBox.Location = new System.Drawing.Point(93, 29);
            this.receivedPacketsTextBox.Name = "receivedPacketsTextBox";
            this.receivedPacketsTextBox.ReadOnly = true;
            this.receivedPacketsTextBox.Size = new System.Drawing.Size(73, 22);
            this.receivedPacketsTextBox.TabIndex = 1;
            this.receivedPacketsTextBox.Text = "0";
            // 
            // transferBytesLabel
            // 
            this.transferBytesLabel.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.transferBytesLabel.AutoSize = true;
            this.transferBytesLabel.Location = new System.Drawing.Point(200, 5);
            this.transferBytesLabel.Name = "transferBytesLabel";
            this.transferBytesLabel.Size = new System.Drawing.Size(56, 16);
            this.transferBytesLabel.TabIndex = 0;
            this.transferBytesLabel.Text = "Tx BPS:";
            // 
            // receivedBytesLabel
            // 
            this.receivedBytesLabel.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.receivedBytesLabel.AutoSize = true;
            this.receivedBytesLabel.Location = new System.Drawing.Point(199, 31);
            this.receivedBytesLabel.Name = "receivedBytesLabel";
            this.receivedBytesLabel.Size = new System.Drawing.Size(57, 16);
            this.receivedBytesLabel.TabIndex = 0;
            this.receivedBytesLabel.Text = "Rx BPS:";
            // 
            // transferredBytesTextBox
            // 
            this.transferredBytesTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.transferredBytesTextBox.Location = new System.Drawing.Point(262, 3);
            this.transferredBytesTextBox.Name = "transferredBytesTextBox";
            this.transferredBytesTextBox.ReadOnly = true;
            this.transferredBytesTextBox.Size = new System.Drawing.Size(74, 22);
            this.transferredBytesTextBox.TabIndex = 2;
            this.transferredBytesTextBox.Text = "0";
            // 
            // receivedBytesTextBox
            // 
            this.receivedBytesTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.receivedBytesTextBox.Location = new System.Drawing.Point(262, 29);
            this.receivedBytesTextBox.Name = "receivedBytesTextBox";
            this.receivedBytesTextBox.ReadOnly = true;
            this.receivedBytesTextBox.Size = new System.Drawing.Size(74, 22);
            this.receivedBytesTextBox.TabIndex = 2;
            this.receivedBytesTextBox.Text = "0";
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 1;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Controls.Add(this.messagesTextBox, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.messageLabel, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel9, 0, 2);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 43);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 3;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 19F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 37F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(345, 235);
            this.tableLayoutPanel3.TabIndex = 1;
            // 
            // messagesTextBox
            // 
            this.messagesTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.messagesTextBox.Location = new System.Drawing.Point(3, 22);
            this.messagesTextBox.Name = "messagesTextBox";
            this.messagesTextBox.ReadOnly = true;
            this.messagesTextBox.Size = new System.Drawing.Size(339, 173);
            this.messagesTextBox.TabIndex = 1;
            this.messagesTextBox.Text = "";
            // 
            // messageLabel
            // 
            this.messageLabel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.messageLabel.AutoSize = true;
            this.messageLabel.Location = new System.Drawing.Point(119, 3);
            this.messageLabel.Name = "messageLabel";
            this.messageLabel.Size = new System.Drawing.Size(107, 16);
            this.messageLabel.TabIndex = 0;
            this.messageLabel.Text = "Message Center";
            // 
            // tableLayoutPanel9
            // 
            this.tableLayoutPanel9.ColumnCount = 2;
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel9.Controls.Add(this.clearMessagesButton, 0, 0);
            this.tableLayoutPanel9.Controls.Add(this.openSessionDirectoryButton, 1, 0);
            this.tableLayoutPanel9.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel9.Location = new System.Drawing.Point(3, 201);
            this.tableLayoutPanel9.Name = "tableLayoutPanel9";
            this.tableLayoutPanel9.RowCount = 1;
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel9.Size = new System.Drawing.Size(339, 31);
            this.tableLayoutPanel9.TabIndex = 2;
            // 
            // clearMessagesButton
            // 
            this.clearMessagesButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.clearMessagesButton.Location = new System.Drawing.Point(22, 3);
            this.clearMessagesButton.Name = "clearMessagesButton";
            this.clearMessagesButton.Size = new System.Drawing.Size(124, 25);
            this.clearMessagesButton.TabIndex = 0;
            this.clearMessagesButton.Text = "Clear Messages";
            this.clearMessagesButton.UseVisualStyleBackColor = true;
            this.clearMessagesButton.Click += new System.EventHandler(this.clearMessagesButton_Click);
            // 
            // openSessionDirectoryButton
            // 
            this.openSessionDirectoryButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.openSessionDirectoryButton.Location = new System.Drawing.Point(192, 3);
            this.openSessionDirectoryButton.Name = "openSessionDirectoryButton";
            this.openSessionDirectoryButton.Size = new System.Drawing.Size(124, 25);
            this.openSessionDirectoryButton.TabIndex = 0;
            this.openSessionDirectoryButton.Text = "Open Directory";
            this.openSessionDirectoryButton.UseVisualStyleBackColor = true;
            this.openSessionDirectoryButton.Click += new System.EventHandler(this.openSessionDirectoryButton_Click);
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel5.ColumnCount = 1;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Controls.Add(this.drivingModeEnableButton, 0, 3);
            this.tableLayoutPanel5.Controls.Add(this.dataCaptureGroupBox, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.tableLayoutPanel10, 0, 2);
            this.tableLayoutPanel5.Controls.Add(this.tiltPanel, 0, 1);
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 4;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 195F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(299, 361);
            this.tableLayoutPanel5.TabIndex = 0;
            // 
            // drivingModeEnableButton
            // 
            this.drivingModeEnableButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.drivingModeEnableButton.Location = new System.Drawing.Point(87, 334);
            this.drivingModeEnableButton.Name = "drivingModeEnableButton";
            this.drivingModeEnableButton.Size = new System.Drawing.Size(124, 24);
            this.drivingModeEnableButton.TabIndex = 4;
            this.drivingModeEnableButton.UseVisualStyleBackColor = true;
            this.drivingModeEnableButton.Click += new System.EventHandler(this.drivingModeEnableButton_Click);
            // 
            // dataCaptureGroupBox
            // 
            this.dataCaptureGroupBox.Controls.Add(this.tableLayoutPanel6);
            this.dataCaptureGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dataCaptureGroupBox.Location = new System.Drawing.Point(3, 3);
            this.dataCaptureGroupBox.Name = "dataCaptureGroupBox";
            this.dataCaptureGroupBox.Size = new System.Drawing.Size(293, 189);
            this.dataCaptureGroupBox.TabIndex = 1;
            this.dataCaptureGroupBox.TabStop = false;
            this.dataCaptureGroupBox.Text = "Data Capture";
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tableLayoutPanel6.ColumnCount = 1;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.Controls.Add(this.dataFileNameTextBox, 0, 3);
            this.tableLayoutPanel6.Controls.Add(this.tableLayoutPanel7, 0, 0);
            this.tableLayoutPanel6.Controls.Add(this.tableLayoutPanel8, 0, 1);
            this.tableLayoutPanel6.Controls.Add(this.dataFileNameLabel, 0, 2);
            this.tableLayoutPanel6.Controls.Add(this.generateFileNameCheckBox, 0, 4);
            this.tableLayoutPanel6.Location = new System.Drawing.Point(3, 18);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 5;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 57F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 27F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(287, 164);
            this.tableLayoutPanel6.TabIndex = 0;
            // 
            // dataFileNameTextBox
            // 
            this.dataFileNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.dataFileNameTextBox.Location = new System.Drawing.Point(10, 115);
            this.dataFileNameTextBox.Margin = new System.Windows.Forms.Padding(10, 3, 10, 3);
            this.dataFileNameTextBox.Name = "dataFileNameTextBox";
            this.dataFileNameTextBox.Size = new System.Drawing.Size(267, 22);
            this.dataFileNameTextBox.TabIndex = 3;
            this.dataFileNameTextBox.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // tableLayoutPanel7
            // 
            this.tableLayoutPanel7.ColumnCount = 2;
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 67.5F));
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 32.5F));
            this.tableLayoutPanel7.Controls.Add(this.startCaptureButton, 1, 0);
            this.tableLayoutPanel7.Controls.Add(this.captureTypeList, 0, 0);
            this.tableLayoutPanel7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel7.Location = new System.Drawing.Point(3, 3);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 1;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel7.Size = new System.Drawing.Size(281, 29);
            this.tableLayoutPanel7.TabIndex = 0;
            // 
            // startCaptureButton
            // 
            this.startCaptureButton.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.startCaptureButton.Location = new System.Drawing.Point(192, 3);
            this.startCaptureButton.Name = "startCaptureButton";
            this.startCaptureButton.Size = new System.Drawing.Size(83, 23);
            this.startCaptureButton.TabIndex = 3;
            this.startCaptureButton.Text = "Start";
            this.startCaptureButton.UseVisualStyleBackColor = true;
            this.startCaptureButton.Click += new System.EventHandler(this.startCaptureButton_Click);
            // 
            // captureTypeList
            // 
            this.captureTypeList.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.captureTypeList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.captureTypeList.FormattingEnabled = true;
            this.captureTypeList.Location = new System.Drawing.Point(19, 3);
            this.captureTypeList.Name = "captureTypeList";
            this.captureTypeList.Size = new System.Drawing.Size(151, 24);
            this.captureTypeList.TabIndex = 2;
            // 
            // tableLayoutPanel8
            // 
            this.tableLayoutPanel8.ColumnCount = 2;
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 51.5F));
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 48.5F));
            this.tableLayoutPanel8.Controls.Add(this.label3, 0, 0);
            this.tableLayoutPanel8.Controls.Add(this.label2, 1, 0);
            this.tableLayoutPanel8.Controls.Add(this.captureRateTextBox, 0, 1);
            this.tableLayoutPanel8.Controls.Add(this.captureDurationTextBox, 1, 1);
            this.tableLayoutPanel8.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel8.Location = new System.Drawing.Point(3, 38);
            this.tableLayoutPanel8.Name = "tableLayoutPanel8";
            this.tableLayoutPanel8.RowCount = 2;
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel8.Size = new System.Drawing.Size(281, 51);
            this.tableLayoutPanel8.TabIndex = 1;
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 2);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(117, 16);
            this.label3.TabIndex = 5;
            this.label3.Text = "Sample Rate (Hz):";
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(165, 2);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(94, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "Duration (sec):";
            // 
            // captureRateTextBox
            // 
            this.captureRateTextBox.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.captureRateTextBox.Location = new System.Drawing.Point(46, 24);
            this.captureRateTextBox.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.captureRateTextBox.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.captureRateTextBox.Name = "captureRateTextBox";
            this.captureRateTextBox.Size = new System.Drawing.Size(51, 22);
            this.captureRateTextBox.TabIndex = 6;
            this.captureRateTextBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // captureDurationTextBox
            // 
            this.captureDurationTextBox.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.captureDurationTextBox.AutoSize = true;
            this.captureDurationTextBox.Location = new System.Drawing.Point(186, 24);
            this.captureDurationTextBox.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.captureDurationTextBox.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.captureDurationTextBox.Name = "captureDurationTextBox";
            this.captureDurationTextBox.Size = new System.Drawing.Size(52, 22);
            this.captureDurationTextBox.TabIndex = 7;
            this.captureDurationTextBox.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // dataFileNameLabel
            // 
            this.dataFileNameLabel.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.dataFileNameLabel.AutoSize = true;
            this.dataFileNameLabel.Location = new System.Drawing.Point(91, 94);
            this.dataFileNameLabel.Name = "dataFileNameLabel";
            this.dataFileNameLabel.Size = new System.Drawing.Size(105, 16);
            this.dataFileNameLabel.TabIndex = 2;
            this.dataFileNameLabel.Text = "Data File Name:";
            // 
            // generateFileNameCheckBox
            // 
            this.generateFileNameCheckBox.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.generateFileNameCheckBox.AutoSize = true;
            this.generateFileNameCheckBox.Location = new System.Drawing.Point(69, 142);
            this.generateFileNameCheckBox.Name = "generateFileNameCheckBox";
            this.generateFileNameCheckBox.Size = new System.Drawing.Size(148, 19);
            this.generateFileNameCheckBox.TabIndex = 4;
            this.generateFileNameCheckBox.Text = "Generate File Name";
            this.generateFileNameCheckBox.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel10
            // 
            this.tableLayoutPanel10.ColumnCount = 2;
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 55.2901F));
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 44.7099F));
            this.tableLayoutPanel10.Controls.Add(this.tiltAngleTextBox, 1, 0);
            this.tableLayoutPanel10.Controls.Add(this.tiltAngleLabel, 0, 0);
            this.tableLayoutPanel10.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel10.Location = new System.Drawing.Point(3, 302);
            this.tableLayoutPanel10.Name = "tableLayoutPanel10";
            this.tableLayoutPanel10.RowCount = 1;
            this.tableLayoutPanel10.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel10.Size = new System.Drawing.Size(293, 26);
            this.tableLayoutPanel10.TabIndex = 2;
            // 
            // tiltAngleTextBox
            // 
            this.tiltAngleTextBox.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.tiltAngleTextBox.Location = new System.Drawing.Point(164, 3);
            this.tiltAngleTextBox.Name = "tiltAngleTextBox";
            this.tiltAngleTextBox.ReadOnly = true;
            this.tiltAngleTextBox.Size = new System.Drawing.Size(50, 22);
            this.tiltAngleTextBox.TabIndex = 3;
            this.tiltAngleTextBox.Text = "0";
            // 
            // tiltAngleLabel
            // 
            this.tiltAngleLabel.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.tiltAngleLabel.AutoSize = true;
            this.tiltAngleLabel.Location = new System.Drawing.Point(56, 5);
            this.tiltAngleLabel.Name = "tiltAngleLabel";
            this.tiltAngleLabel.Size = new System.Drawing.Size(102, 16);
            this.tiltAngleLabel.TabIndex = 0;
            this.tiltAngleLabel.Text = "Tilt Angle (deg):";
            // 
            // tiltPanel
            // 
            this.tiltPanel.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.tiltPanel.Location = new System.Drawing.Point(49, 198);
            this.tiltPanel.Name = "tiltPanel";
            this.tiltPanel.Size = new System.Drawing.Size(200, 98);
            this.tiltPanel.TabIndex = 3;
            this.tiltPanel.Paint += new System.Windows.Forms.PaintEventHandler(this.tiltImagePanel_Paint);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(660, 407);
            this.Controls.Add(this.mainTableLayoutPanel);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.Text = "EvaUI";
            this.mainTableLayoutPanel.ResumeLayout(false);
            this.outputDirectoryTableLayoutPanel.ResumeLayout(false);
            this.outputDirectoryTableLayoutPanel.PerformLayout();
            this.displaySplitContainer.Panel1.ResumeLayout(false);
            this.displaySplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.displaySplitContainer)).EndInit();
            this.displaySplitContainer.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.linkStatsGroupBox.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tableLayoutPanel9.ResumeLayout(false);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.dataCaptureGroupBox.ResumeLayout(false);
            this.tableLayoutPanel6.ResumeLayout(false);
            this.tableLayoutPanel6.PerformLayout();
            this.tableLayoutPanel7.ResumeLayout(false);
            this.tableLayoutPanel8.ResumeLayout(false);
            this.tableLayoutPanel8.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.captureRateTextBox)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.captureDurationTextBox)).EndInit();
            this.tableLayoutPanel10.ResumeLayout(false);
            this.tableLayoutPanel10.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel mainTableLayoutPanel;
        private System.Windows.Forms.TableLayoutPanel outputDirectoryTableLayoutPanel;
        private System.Windows.Forms.Label outputDirectoryLabel;
        private System.Windows.Forms.Button outputDirectoryBrowseButton;
        private System.Windows.Forms.TextBox outputDirectoryTextBox;
        private System.Windows.Forms.SplitContainer displaySplitContainer;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.Button connectButton;
        private System.Windows.Forms.GroupBox linkStatsGroupBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.Label messageLabel;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.Label transferedPacketsLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox transferedPacketsTextBox;
        private System.Windows.Forms.TextBox receivedPacketsTextBox;
        private System.Windows.Forms.Button refreshPortsButton;
        private System.Windows.Forms.RichTextBox messagesTextBox;
        private System.Windows.Forms.ComboBox portBox;
        private System.Windows.Forms.Label transferBytesLabel;
        private System.Windows.Forms.Label receivedBytesLabel;
        private System.Windows.Forms.TextBox transferredBytesTextBox;
        private System.Windows.Forms.TextBox receivedBytesTextBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.ComboBox captureTypeList;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox dataCaptureGroupBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.Button startCaptureButton;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel8;
        private System.Windows.Forms.Label dataFileNameLabel;
        private System.Windows.Forms.TextBox dataFileNameTextBox;
        private System.Windows.Forms.CheckBox generateFileNameCheckBox;
        private System.Windows.Forms.NumericUpDown captureRateTextBox;
        private System.Windows.Forms.NumericUpDown captureDurationTextBox;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel9;
        private System.Windows.Forms.Button clearMessagesButton;
        private System.Windows.Forms.Button openSessionDirectoryButton;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel10;
        private System.Windows.Forms.Label tiltAngleLabel;
        private System.Windows.Forms.TextBox tiltAngleTextBox;
        private System.Windows.Forms.Panel tiltPanel;
        private System.Windows.Forms.Button drivingModeEnableButton;
    }
}

