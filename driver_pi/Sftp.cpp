
#include "Sftp.hpp"

#if BUILD_TARGET == TARGET_PI3

static const char* TAG = "Sftp";

void write_log(const std::string& strLogMsg) {
  log_i(TAG, "con: %s", strLogMsg.c_str());
}

Sftp::Sftp() {}

bool Sftp::connect(std::string host, uint16_t port, std::string user,
                   std::string pw) {
  // CFTPClient sftp = CFTPClient(write_log);
  sftp.reset(new CFTPClient(write_log));
  bool loginSuc =
      sftp->InitSession(host, port, user, pw, CFTPClient::FTP_PROTOCOL::SFTP,
                        CFTPClient::SettingsFlag::ENABLE_LOG);
  log_i(TAG, "loged in: %d", loginSuc);
  sftp->SetInsecure(true);
  return loginSuc;
}

bool Sftp::disconnect() { return sftp->CleanupSession(); }

// curl -k "sftp://81.169.224.239/files/" --user "ftpLogUser:q5+rUPu=R4wX-?f$"
// -T "logs/2021-05-10_08-07-45.log" --ftp-create-dirs

bool Sftp::upload_folder(std::string local_path, std::string remote_path,
                         bool delete_after_upload,
                         std::vector<std::string> delete_exceptions) {
  // std::string base_path = "files";  //"/home/ftpLogUser";
  bool total_suc = true;
  bool create_dirs = true;
  for (const auto& entry : std::filesystem::directory_iterator(local_path)) {
    // std::cout << entry.path() << std::endl;
    bool delete_this =
        delete_after_upload &&
        std::find(delete_exceptions.begin(), delete_exceptions.end(),
                  entry.path().string()) == delete_exceptions.end();
    if (!this->upload_file(
            entry.path(),
            remote_path,  // + "/" + entry.path().filename().string(),
            delete_this)) {
      total_suc = false;
    }
    /*
try {
bool suc = sftp->UploadFile(entry.path(),
base_path + "/" + remote_path + "/" +
    entry.path().filename().string(),
&create_dirs);
log_i(TAG, "uploaded %s -> %s: %d", entry.path().c_str(),
(base_path + "/" + remote_path + "/" +
entry.path().filename().string())
.c_str(),
suc);
if (suc) {
if (delete_after_upload &&
std::find(delete_exceptions.begin(), delete_exceptions.end(),
entry.path().string()) == delete_exceptions.end()) {
log_d(TAG, "delete %s", entry.path().c_str());
remove(entry.path());
}
} else {
total_suc = false;
}
} catch (...) {
return false;
}
*/
  }
  return total_suc;
}

bool Sftp::upload_file(std::string local_path, std::string remote_path,
                       bool delete_after_upload) {
  std::string base_path = "files";
  bool create_dirs = true;
  try {
    bool suc =
        sftp->UploadFile(local_path.c_str(),
                         base_path + "/" + remote_path.c_str(), &create_dirs);
    log_i(TAG, "uploaded %s -> %s: %d", local_path.c_str(), remote_path.c_str(),
          suc);
    if (suc) {
      if (delete_after_upload) {
        log_d(TAG, "delete %s", local_path.c_str());
        remove(local_path.c_str());
      }
      return true;
    } else {
      return false;
    }
  } catch (...) {
    return false;
  }
  return false;
}

bool Sftp::download_file(std::string local_path, std::string remote_path) {
  std::string base_path = "files";
  try {
    CFTPClient::FileInfo res_file_info = {0, 0.0};
    bool sucInfo = sftp->Info(base_path + "/" + remote_path, res_file_info);
    if (!sucInfo) {
      return false;
    }
    bool suc = sftp->DownloadFile(local_path, base_path + "/" + remote_path);
    log_i(TAG, "downloaded %s -> %s: %d",
          (base_path + "/" + remote_path).c_str(), local_path.c_str(), suc);
    return suc;
  } catch (...) {
    return false;
  }
  return false;
}

#endif
