from selenium import webdriver
from selenium.webdriver.common.by import By
from sentence_transformers import SentenceTransformer, util

# Preparing BERT model for our comparison
bert_model = SentenceTransformer('dbmdz/bert-base-turkish-cased')

# Since using safari for selenium we should adjust a webdriver for safari
driver = webdriver.Safari()

wait = WebDriverWait(driver, 15)
time.sleep(3)  # waiting for site to fully download itself


print("\n--- Sales Bot Autonomous System ---")
link1 = input("Please insert your url for first product: ")
link2 = input("Please insert your url for second product: ")


def product_data_pull(url, name_css, price_css):
    driver.get(url)
    # since some sites are not refreshed fast enough while selenium is working, we have to adjust
    # our own waiting and refreshing time
    name = driver.find_element(By.CSS_SELECTOR, name_css).text
    price = driver.find_element(By.CSS_SELECTOR, price_css).text
    return name, price


try:
    link1_name, link1_price = product_data_pull(driver, link1)
    link2_name, link2_price = product_data_pull(driver, link2)

    # Using BERT for comparing names of the products using cosine similarity

    emb1 = bert_model.encode(link1_name, convert_to_tensor=True)
    emb2 = bert_model.encode(link2_name, convert_to_tensor=True)
    Score = util.cos_sim(emb1, emb2).item()

    print(f"Product 1: {link1_name} ({link1_price})")
    print(f"Product 1: {link2_name} ({link2_price})")

    print(f"Similarity Score: %{Score*100:.2f}")

    # If cosine similarity is greater than 0.85 which is multiplied with 100  after, then the name
    # of the products are same with some minor differences, this is the result of BERT.
    if Score > 0.85:
        print(
            f"Comparison is done, Price: {link1_name}({link2_price}) - {link2_name}({link2_price})")

finally:
    driver.quit()


def clean_price(price_Tag):
    clean_priceTag = price_Tag.replace("TL", "").replace(
        ".", "").replace(",", ".").strip()
    return float(clean_priceTag)


link1_number = clean_price(link1_price)
link2_number = clean_price(link2_price)

if link1_number < link2_number:
    print(f"{link2_name} with {link2_number} is more expensive than {link1_name} with {link1_number}")

else:
    print(f"{link2_name} with {link2_number} is cheaper than {link1_name} with {link1_number}")


# except Exception as error:
    # print(f"error occured {error}")
