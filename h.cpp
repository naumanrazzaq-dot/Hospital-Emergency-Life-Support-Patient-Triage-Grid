#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Base Abstract Class
class PatientMonitor {
protected:
    string patientID;
    int heartRateBPM;
    double oxygenSaturationSpO2;

public:
    static int activeMonitoredBeds;
    static int criticalAlertCount;

    PatientMonitor(string id, int hr, double spo2)
        : patientID(id), heartRateBPM(hr), oxygenSaturationSpO2(spo2) {
        activeMonitoredBeds++;
    }

    virtual ~PatientMonitor() {
        cout << "[DISCHARGE] Patient " << patientID << " monitor disconnected. Bed open." << endl;
        activeMonitoredBeds--;
    }

    // Pure Virtual Interfaces
    virtual double calculateCriticalSeverityIndex() const = 0;
    virtual void printVitalTelemetry() const = 0;
};

// Static definitions outside class
int PatientMonitor::activeMonitoredBeds = 0;
int PatientMonitor::criticalAlertCount = 0;

// Derived Class 1: Intensive Care Unit (ICU) Invasive Monitor
class ICUPatientMonitor : public PatientMonitor {
private:
    double arterialBloodPressure; // Mean Arterial Pressure (MAP)
    bool onMechanicalVentilator;

public:
    ICUPatientMonitor(string id, int hr, double spo2, double arterialPressure, bool ventilator)
        : PatientMonitor(id, hr, spo2),
          arterialBloodPressure(arterialPressure),
          onMechanicalVentilator(ventilator) {}

    ~ICUPatientMonitor() override {
        cout << " -> Sterilizing ICU sensor arterial line for " << patientID << "..." << endl;
    }

    double calculateCriticalSeverityIndex() const override {
        // High heart rate variance or low MAP increases risk score
        double severity = (100.0 - oxygenSaturationSpO2) * 2.5;
        if (heartRateBPM > 120 || heartRateBPM < 50) severity += 25.0;
        if (arterialBloodPressure < 65.0) severity += 30.0;
        if (onMechanicalVentilator) severity += 15.0;
        return severity;
    }

    void printVitalTelemetry() const override {
        double score = calculateCriticalSeverityIndex();
        if (score >= 40.0) PatientMonitor::criticalAlertCount++;

        cout << "\n==============================================" << endl;
        cout << "   ICU VITAL TELEMETRY: " << patientID << endl;
        cout << "==============================================" << endl;
        cout << "  Heart Rate       : " << heartRateBPM << " BPM" << endl;
        cout << "  Oxygen Saturation: " << fixed << setprecision(1) << oxygenSaturationSpO2 << " %" << endl;
        cout << "  Arterial MAP     : " << arterialBloodPressure << " mmHg" << endl;
        cout << "  Ventilator Hook  : " << (onMechanicalVentilator ? "YES (ACTIVE)" : "NO") << endl;
        cout << "  Severity Score   : " << fixed << setprecision(2) << score << " / 100" << endl;
        cout << "  Status           : " << (score >= 40.0 ? "CRITICAL HAZARD" : "STABLE") << endl;
        cout << "==============================================" << endl;
    }
};

// Derived Class 2: General Ward Wireless Telemetry Patch
class WardTelemetryPatch : public PatientMonitor {
private:
    double bodyTemperatureCelsius;
    double batteryLevelPct;

public:
    WardTelemetryPatch(string id, int hr, double spo2, double temp, double battery)
        : PatientMonitor(id, hr, spo2),
          bodyTemperatureCelsius(temp),
          batteryLevelPct(battery) {}

    ~WardTelemetryPatch() override {
        cout << " -> Powering down wireless patch transmitter for " << patientID << "..." << endl;
    }

    double calculateCriticalSeverityIndex() const override {
        double severity = (100.0 - oxygenSaturationSpO2) * 1.5;
        if (bodyTemperatureCelsius >= 38.5) severity += 20.0;
        if (heartRateBPM > 100) severity += 10.0;
        return severity;
    }

    void printVitalTelemetry() const override {
        double score = calculateCriticalSeverityIndex();
        if (score >= 30.0) PatientMonitor::criticalAlertCount++;

        cout << "\n==============================================" << endl;
        cout << "   WARD TELEMETRY PATCH: " << patientID << endl;
        cout << "==============================================" << endl;
        cout << "  Heart Rate       : " << heartRateBPM << " BPM" << endl;
        cout << "  Oxygen Saturation: " << fixed << setprecision(1) << oxygenSaturationSpO2 << " %" << endl;
        cout << "  Body Temperature : " << bodyTemperatureCelsius << " °C" << endl;
        cout << "  Patch Battery    : " << batteryLevelPct << " %" << endl;
        cout << "  Severity Score   : " << fixed << setprecision(2) << score << " / 100" << endl;
        cout << "  Status           : " << (score >= 30.0 ? "ATTENTION NEEDED" : "NORMAL") << endl;
        cout << "==============================================" << endl;
    }
};

int main() {
    cout << "\n>>> HOSPITAL CENTRAL MONITORING SYSTEM BOOT <<<\n" << endl;

    const int CAPACITY = 2;
    PatientMonitor* wardBeds[CAPACITY];

    // Bed 1: ICU patient in critical distress (HR: 135, SpO2: 89.5%, MAP: 58.0, On Ventilator: True)
    wardBeds[0] = new ICUPatientMonitor("PATIENT-ICU-88", 135, 89.5, 58.0, true);

    // Bed 2: General ward stable recovery (HR: 76, SpO2: 98.0%, Temp: 37.1°C, Battery: 94%)
    wardBeds[1] = new WardTelemetryPatch("PATIENT-WARD-14", 76, 98.0, 37.1, 94.0);

    // Polymorphic display loop
    for (int i = 0; i < CAPACITY; i++) {
        wardBeds[i]->printVitalTelemetry();
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Monitored Patients : " << PatientMonitor::activeMonitoredBeds << endl;
    cout << "Critical Risk Alarms      : " << PatientMonitor::criticalAlertCount << endl;
    cout << "----------------------------------------------\n" << endl;

    cout << ">>> COMMENCING PATIENT DISCHARGE & SHUTDOWN <<<\n" << endl;

    // Proper dynamic heap cleanup
    for (int i = 0; i < CAPACITY; i++) {
        delete wardBeds[i];
        wardBeds[i] = nullptr;
    }

    cout << "\n----------------------------------------------" << endl;
    cout << "Active Monitored Patients After Teardown : " << PatientMonitor::activeMonitoredBeds << endl;
    cout << "----------------------------------------------" << endl;

    return 0;
}
