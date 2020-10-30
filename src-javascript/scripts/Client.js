'use strict'

class Client
{
  constructor(address, port, apiUri)
  {
    this._serverUrl = "http://" + address + ":" + port + apiUri;
  }

  sendEmail(to_email, from_email, subject, body)
  {
    const requestBody = {
      to_email: to_email,
      from_email: from_email,
      subject: subject,
      body: body,
    };

    return this._xmlHttpRequest({
      method: "POST",
      uri: this._serverUrl,
      body: JSON.stringify(requestBody),
      headers: [
        {
          name: "Content-Type",
          value: "application/json", 
        },
      ],
    });
  }

  getEmail(id)
  {
    return this._xmlHttpRequest({
      method: "GET",
      uri: this._serverUrl + "?id=" + id,
    });
  }

  getEmailStatus(id)
  {
    return this._xmlHttpRequest({
      method: "GET",
      uri: this._serverUrl + "?id=" + id + "&status_only",
    });
  }

  deleteEmail(id)
  {
    return this._xmlHttpRequest({
      method: "DELETE",
      uri: this._serverUrl + "?id=" + id,
    });
  }

  _xmlHttpRequest(options)
  {
    return new Promise((resolve, reject) => {
      const loadingElem = document.getElementById("loading");
      loadingElem.setAttribute(
          "style", "background-image: url(images/spinner.gif);");

      const xmlHttp = new XMLHttpRequest();
      xmlHttp.onreadystatechange = () => 
      {
        loadingElem.setAttribute("style", "");
        
        if (xmlHttp.readyState === XMLHttpRequest.DONE && xmlHttp.status === 200)
        {
          resolve(JSON.parse(xmlHttp.responseText));
        }
        else if(xmlHttp.readyState === XMLHttpRequest.DONE)
        {
          reject("Error: " + xmlHttp.status + " " + xmlHttp.statusText);
        }
      }

      xmlHttp.open(options.method, options.uri, true);

      if (options.headers)
      {
        for (const header of options.headers)
        {
          xmlHttp.setRequestHeader(header.name, header.value);
        }
      }

      xmlHttp.send(options.body);
    });
  }
}

export {Client};