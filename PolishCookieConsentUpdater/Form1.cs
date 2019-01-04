using System;
using System.ComponentModel;
using System.Net;
using System.Windows.Forms;
using System.IO;
using System.Linq;
using System.Resources;
using Ionic.Zip;
using Octokit;
using Newtonsoft.Json;
using System.Speech.Synthesis;
using Ookii.Dialogs.WinForms;

namespace PolishCookieConsentUpdater
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            label1.Text = GetInstallPath();
            this.AutoSize = false;
        }

        WebClient webClient;

        string pathDownloadExt = Environment.ExpandEnvironmentVariables(@"%USERPROFILE%\Downloads\PolishCookieConsent");

        ResourceManager rm = new ResourceManager("PolishCookieConsentUpdater.Lang", typeof(Form1).Assembly);

        public string GetInstallPath()
        {
            string pathInstallExt = "";
            if ((string)Properties.Settings.Default["pathInstallExt"] != "")
            {
                pathInstallExt = (string)Properties.Settings.Default["pathInstallExt"];
            }
            else
            {
                pathInstallExt = Environment.ExpandEnvironmentVariables(@"%USERPROFILE%\Rozszerzenia\PolishCookieConsent");
            }
            return pathInstallExt;
        }

        public void DownloadFile(string urlAddress, string location)
        {
            using (webClient = new WebClient())
            {
                ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;
                webClient.DownloadFileCompleted += new AsyncCompletedEventHandler(Completed);
                webClient.DownloadProgressChanged += new DownloadProgressChangedEventHandler(ProgressChanged);

                Uri URL = urlAddress.StartsWith("https://", StringComparison.OrdinalIgnoreCase) ? new Uri(urlAddress) : new Uri("https://" + urlAddress);

                try
                {
                    webClient.DownloadFileAsync(URL, location);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                };
            }
        }

        private void ProgressChanged(object sender, DownloadProgressChangedEventArgs e)
        {
            // Update the progressbar percentage only when the value is not the same.
            progressBar.Value = e.ProgressPercentage;

            // Show the percentage on our label.
            labelPerc.Text = rm.GetString("downloaded") + ": " + e.ProgressPercentage.ToString() + "%";
        }


        private void Completed(object sender, AsyncCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                string error = e.Error.ToString();
                MessageBox.Show(error);
                return;
            }

            if (e.Cancelled == true)
            {
                MessageBox.Show(rm.GetString("canceledDownload"));
            }
            else
            {
                using (ZipFile zip = ZipFile.Read(pathDownloadExt + @"\PolishCookieConsent_chromium.zip"))
                {
                    foreach (ZipEntry f in zip)
                    {
                        f.Extract(GetInstallPath(), ExtractExistingFileAction.OverwriteSilently);
                    }
                }
                Directory.Delete(pathDownloadExt, true);
                SpeechSynthesizer synthesizer = new SpeechSynthesizer();
                synthesizer.Volume = 100;
                synthesizer.SpeakAsync(rm.GetString("completedUpdate"));
                MessageBox.Show(rm.GetString("completedUpdate"));
            }
        }

        public class Manifest
        {
            public string version { get; set; }
        }

        public async void UpdateExt()
        {
            var client = new GitHubClient(new ProductHeaderValue("my-cool-app"));
            var latestReleaseTag = (await client.Repository.Release.GetLatest("PolishFiltersTeam", "PolishCookieConsent")).TagName;
            var newVersion = new Version(latestReleaseTag.Replace("v", ""));

            Manifest manifest = JsonConvert.DeserializeObject<Manifest>(File.ReadAllText(GetInstallPath() + @"\manifest.json"));
            var oldVersion = new Version(manifest.version);

            if (newVersion > oldVersion)
            {
                Directory.CreateDirectory(pathDownloadExt);
                DownloadFile("https://github.com/PolishFiltersTeam/PolishCookieConsent/releases/download/" + latestReleaseTag + "///PolishCookieConsent_chromium.zip", pathDownloadExt + @"\PolishCookieConsent_chromium.zip");
            }
            else
            {
                MessageBox.Show(rm.GetString("noNewerVersion"));
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            UpdateExt();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] passedInArgs = Environment.GetCommandLineArgs();

            if (passedInArgs.Contains("/u"))
            {
                UpdateExt();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            VistaFolderBrowserDialog fbd = new VistaFolderBrowserDialog();
            
            if (fbd.ShowDialog() == DialogResult.OK)
            {
                string installPath = fbd.SelectedPath;
                Properties.Settings.Default["pathInstallExt"] = installPath;
                Properties.Settings.Default.Save();
                label1.Text = installPath;
            }

        }
    }
}
