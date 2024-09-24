#include <c4/format.hpp>
#include <ryml.hpp>
#include <ryml_std.hpp>

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>

enum class ResponseCode {
    OK_SuccessfulNegotiation = 0,
    OK_SuccessfulNegotiationWithMinorDeviation = 1,
    Failed_NoNegotiation = 2,
};

std::string response_code_to_string(ResponseCode response_code) {
    switch (response_code) {
    case ResponseCode::OK_SuccessfulNegotiation:
        return "OK_SuccessfulNegotiation";
    case ResponseCode::OK_SuccessfulNegotiationWithMinorDeviation:
        return "OK_SuccessfulNegotiationWithMinorDeviation";
    case ResponseCode::Failed_NoNegotiation:
        return "Failed_NoNegotiation";
    }
    throw std::out_of_range("No known string conversion for provided enum of type ResponseCode");
}

ResponseCode string_to_response_code(const std::string& s) {
    if (s == "OK_SuccessfulNegotiation") {
        return ResponseCode::OK_SuccessfulNegotiation;
    } else if (s == "OK_SuccessfulNegotiationWithMinorDeviation") {
        return ResponseCode::OK_SuccessfulNegotiationWithMinorDeviation;
    } else if (s == "Failed_NoNegotiation") {
        return ResponseCode::Failed_NoNegotiation;
    }
    throw std::out_of_range("Provided string " + s + " could not be converted to enum of type ResponseCode");
}

struct SupportedAppProtocolResponse {
    ResponseCode response_code;
    std::optional<uint8_t> schema_id;
};

void write(ryml::NodeRef* n, SupportedAppProtocolResponse const& val) {
    *n |= ryml::MAP;
    n->append_child() << ryml::key("ResponseCode") << response_code_to_string(val.response_code);
    if (val.schema_id.has_value()) {
        n->append_child() << ryml::key("SchemaID") << val.schema_id.value();
    }
}

bool read(ryml::ConstNodeRef const& n, SupportedAppProtocolResponse* val) {
    std::string tmp;
    n["ResponseCode"] >> tmp;
    val->response_code = string_to_response_code(tmp);
    if (n.has_child("SchemaID")) {
        val->schema_id.emplace();
        n["SchemaID"] >> val->schema_id.value();
    }
    return true;
}

int main() {

    std::cout << "Hello yaml!" << std::endl;

    SupportedAppProtocolResponse in{ResponseCode::OK_SuccessfulNegotiation, 1};

    ryml::csubstr json = R"({
        "ResponseCode": "OK_SuccessfulNegotiation",
        "SchemaID": 1
    })";

    SupportedAppProtocolResponse out;

    ryml::Tree t = ryml::parse_json_in_arena(json);

    std::cout << ryml::as_json(t) << std::endl;

    ryml::Tree t2;
    t2.rootref() << in;
    std::cout << ryml::as_json(t2) << std::endl;

    t.crootref() >> out;

    std::cout << "ResponseCode: " << response_code_to_string(out.response_code)
              << ", SchemaID: " << static_cast<uint32_t>(out.schema_id.value_or(0));

    return 0;
}
