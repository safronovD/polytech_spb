package com.example.demo;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

@SpringBootApplication
public class MyServerApplication {

	public static void main(String[] args) {
		SpringApplication.run(MyServerApplication.class, args);
	}

	@RestController
	@RequestMapping(value = "/server")
	public class ServerController {
		@RequestMapping(value = "/data", method = RequestMethod.GET)
		public String getData() {
			System.out.println("Returning data from server");
			return "Hello from server";
		}
	}
}
