class HostIp {
private:
	static HostIp* hostip;
public:
	static HostIp* Instance() {
		if (hostip == nullptr) {
			hostip = new HostIp();
		}
		return hostip;
	}
};


class Connect {
public:
	virtual void ConnectRequest(std::string ip) const = 0;
};

class ConnectToHost : public Connect {
public:
	void ConnectRequest(std::string ip) const override {
		std::cout << "Connecting to " << ip << std::endl;
	}
	/*
	Тут будуть основні функції для підтримування зв'язку та надсилання інформації.
	*/
};

class Proxy : public Connect {

private:
	ConnectToHost* real_subject_;

	bool CheckConnection() const {
		std::cout << "Verifying connection to the network";
		return true;
	}
	
public:
	Proxy(ConnectToHost* real_subject) : real_subject_(new ConnectToHost(*real_subject)) {
	}
	~Proxy() {
		delete real_subject_;
	}
	void ConnectRequest(std::string ip) const override {
		if (this->CheckConnection()) {
			this->real_subject_->ConnectRequest(ip);
		}
	}
};
void connect(const Connect& subject, const std::string& ip) {
	subject.ConnectRequest(ip);
}

int main() {
	ConnectToHost* conn = new ConnectToHost;
	Proxy* proxy = new Proxy(conn);
	connect(*proxy, "127.0.0.1");
}








