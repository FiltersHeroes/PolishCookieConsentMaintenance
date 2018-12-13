using System;
using System.ComponentModel;
using System.Net;
using System.Windows.Forms;
using System.IO;
using Ionic.Zip;
using Octokit;
using Newtonsoft.Json;
using System.Reflection;

namespace PolishCookieConsentUpdater
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            UpdateExt();
        }

        WebClient webClient;

        string pathDownloadExt = Environment.ExpandEnvironmentVariables(@"%USERPROFILE%\Downloads\PolishCookieConsent");


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
            labelPerc.Text = "Pobrano: " + e.ProgressPercentage.ToString() + "%";
        }

        private void Completed(object sender, AsyncCompletedEventArgs e)
        {
            var fileTxt = Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath) + @"\install.txt";
            string pathInstallExt = "";
            if (File.Exists(fileTxt))
            {

                pathInstallExt = File.ReadAllText(fileTxt);
            }
            else
            {
                pathInstallExt = Environment.ExpandEnvironmentVariables(@"%USERPROFILE%\Rozszerzenia\PolishCookieConsent");
            }
            if (e.Error != null)
            {
                string error = e.Error.ToString();
                MessageBox.Show(error);
                return;
            }

            if (e.Cancelled == true)
            {
                MessageBox.Show("Pobieranie zostało zatrzymane.");
            }
            else
            {
                using (ZipFile zip = ZipFile.Read(pathDownloadExt + @"\PolishCookieConsent_chromium.zip"))
                {
                    foreach (ZipEntry f in zip)
                    {
                        f.Extract(pathInstallExt, ExtractExistingFileAction.OverwriteSilently);
                    }
                }
                Directory.Delete(pathDownloadExt, true);
                MessageBox.Show("Aktualizacja ukończona!");
            }
        }

        public class Manifest
        {
            public string version { get; set; }
        }

        public async void UpdateExt()
        {
            var fileTxt = Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath) +@"\install.txt";
            string pathInstallExt = "";
            if (File.Exists(fileTxt))
            {

                pathInstallExt = File.ReadAllText(fileTxt);
            }
            else
            {
                pathInstallExt = Environment.ExpandEnvironmentVariables(@"%USERPROFILE%\Rozszerzenia\PolishCookieConsent");
            }
            var client = new GitHubClient(new ProductHeaderValue("my-cool-app"));
            var latestReleaseTag = (await client.Repository.Release.GetLatest("PolishFiltersTeam", "PolishCookieConsent")).TagName;
            var newVersion = new Version(latestReleaseTag.Replace("v", ""));


            Manifest manifest = JsonConvert.DeserializeObject<Manifest>(File.ReadAllText(pathInstallExt + @"\manifest.json"));
            var oldVersion = new Version(manifest.version);

            if (newVersion > oldVersion)
            {
                Directory.CreateDirectory(pathDownloadExt);
                DownloadFile("https://github.com/PolishFiltersTeam/PolishCookieConsent/releases/download/" + latestReleaseTag + "///PolishCookieConsent_chromium.zip", pathDownloadExt + @"\PolishCookieConsent_chromium.zip");
            }
            else
            {
                MessageBox.Show("Aktualnie nie ma nowszej wersji rozszerzenia Polska Ciasteczkowa Zgoda.");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            UpdateExt();
        }

    }
}
